import os
import sys
import numpy as np
import pandas as pd
from sklearn.ensemble import RandomForestRegressor, GradientBoostingRegressor
from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score

# Ensure stdout handles UTF-8 output on Windows
if sys.platform == "win32":
    sys.stdout.reconfigure(encoding='utf-8')

def run_ml_pipeline():
    # --- 1. Φόρτωση & Προεπεξεργασία (Preprocessing) ---
    csv_path = os.path.join("data", "ocean.csv")
    if not os.path.exists(csv_path):
        csv_path = "ocean.csv"
        
    print(f"Loading dataset from: {csv_path}")
    df = pd.read_csv(csv_path)

    # Standardize column names if needed
    rename_map = {'Date': 'date', 'T_degC': 'temperature', 'PO4uM': 'phosphate'}
    df = df.rename(columns=rename_map)

    # Convert date to datetime and sort chronologically
    df["date"] = pd.to_datetime(df["date"], format="%m/%d/%Y", errors='coerce')
    df = df.sort_values("date").reset_index(drop=True)
    df.set_index("date", inplace=True)

    print(f"Dataset loaded: {len(df)} records from {df.index.min().strftime('%Y-%m-%d')} to {df.index.max().strftime('%Y-%m-%d')}")

    # --- 2. Feature Engineering (Lag Features) ---
    def create_lags(data, lags=[1, 2, 3]):
        df_lagged = data.copy()
        for l in lags:
            df_lagged[f"temperature_lag_{l}"] = df_lagged["temperature"].shift(l)
            df_lagged[f"phosphate_lag_{l}"] = df_lagged["phosphate"].shift(l)
        return df_lagged

    df_ml = create_lags(df[["temperature", "phosphate"]], lags=[1, 2, 3])
    df_ml = df_ml.dropna()

    X = df_ml.drop(columns=["temperature"])
    y = df_ml["temperature"]

    # --- 3. Train / Test Split (Sequential Time-Series Split) ---
    split_index = int(len(X) * 0.8)
    X_train, X_test = X.iloc[:split_index], X.iloc[split_index:]
    y_train, y_test = y.iloc[:split_index], y.iloc[split_index:]

    print(f"Train samples: {len(X_train)} | Test samples: {len(X_test)}")

    # --- 4. Μοντελοποίηση (Modeling) ---
    print("\n--- Training Random Forest Regressor ---")
    rf_model = RandomForestRegressor(n_estimators=100, random_state=42)
    rf_model.fit(X_train, y_train)

    rf_preds = rf_model.predict(X_test)
    rf_mse = mean_squared_error(y_test, rf_preds)
    rf_rmse = np.sqrt(rf_mse)
    rf_mae = mean_absolute_error(y_test, rf_preds)
    rf_r2 = r2_score(y_test, rf_preds)

    print(f"Random Forest Test MSE:  {rf_mse:.4f}")
    print(f"Random Forest Test RMSE: {rf_rmse:.4f}")
    print(f"Random Forest Test MAE:  {rf_mae:.4f}")
    print(f"Random Forest Test R2:   {rf_r2:.4f}")

    print("\n--- Training Gradient Boosting Regressor ---")
    gb_model = GradientBoostingRegressor(n_estimators=100, random_state=42)
    gb_model.fit(X_train, y_train)

    gb_preds = gb_model.predict(X_test)
    gb_mse = mean_squared_error(y_test, gb_preds)
    gb_rmse = np.sqrt(gb_mse)
    gb_mae = mean_absolute_error(y_test, gb_preds)
    gb_r2 = r2_score(y_test, gb_preds)

    print(f"Gradient Boosting Test MSE:  {gb_mse:.4f}")
    print(f"Gradient Boosting Test RMSE: {gb_rmse:.4f}")
    print(f"Gradient Boosting Test MAE:  {gb_mae:.4f}")
    print(f"Gradient Boosting Test R2:   {gb_r2:.4f}")

    # Feature Importance Analysis
    print("\n--- Feature Importances (Random Forest) ---")
    for feature, importance in zip(X.columns, rf_model.feature_importances_):
        print(f"  {feature:20s}: {importance*100:.2f}%")

if __name__ == "__main__":
    run_ml_pipeline()
