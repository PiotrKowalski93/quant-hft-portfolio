import yfinance as yf
import pandas as pd
import matplotlib.pyplot as plt

# Download APPL quotes
ticker = "AAPL"
data = yf.download(ticker, start="2023-01-01", end="2025-01-01")

if data.empty:
    raise ValueError(f"Brak danych dla tickera: {ticker}")

# Test columns returned from Yahoo Finance
#print(data.columns)
print(data.head(5))
print()

# Daily change 
data['DReturn'] = data['Close'].pct_change()

# Drop first NaN row
data = data.dropna()
print(data[['Close', 'DReturn']].head())

# Moving avg for 20 and 50 days
data['SMA20'] = data['Close'].rolling(window=20).mean()
data['SMA50'] = data['Close'].rolling(window=50).mean()

#print(data['SMA20'])
#print()
#print(data['SMA50'])

# Visualise data
plt.figure(figsize=(12,6))
plt.plot(data['Close'], label='Close')
plt.plot(data['SMA20'], label='SMA20')
plt.plot(data['SMA50'], label='SMA50')
plt.legend()
plt.title(f'{ticker} Price with SMA20 & SMA50')
plt.show()