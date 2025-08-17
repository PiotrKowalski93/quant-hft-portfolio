import yfinance as yf
import pandas as pd
import matplotlib.pyplot as plt

# Download APPL quotes
ticker = "AAPL"
data = yf.download(ticker, start="2023-01-01", end="2025-01-01")

if data.empty:
    raise ValueError(f"Ticker data is empty: {ticker}")

# Test columns returned from Yahoo Finance
#print(data.columns)
print(data.head(5))
print()

# Daily change 
# .pct_change() Fractional change between the current and a prior element. 
data['Daily Return'] = data['Close'].pct_change()

# Drop first NaN row
data = data.dropna()
print(data[['Close', 'Daily Return']].head())

# Simple Moving Avrage for 20 and 50 days
data['SMA20'] = data['Close'].rolling(window=20).mean()
data['SMA50'] = data['Close'].rolling(window=50).mean()

#print(data['SMA20'])
#print()
#print(data['SMA50'])

# Visualise data
# plt.figure(figsize=(12,6))
# plt.plot(data['Close'], label='Close')
# plt.plot(data['SMA20'], label='SMA20')
# plt.plot(data['SMA50'], label='SMA50')
# plt.legend()
# plt.title(f'{ticker} Price with SMA20 & SMA50')
# plt.show()

# Simple strategy
# Buy when SMA20 > SMA50
# Sell otherwise
data['Position'] = 0
data.loc[data['SMA20'] > data['SMA50'], 'Position'] = 1

data['Strategy Return'] = data['Position'].shift(1) * data['Daily Return']

print()
#print(data[['Daily Return', 'Strategy Return']])

# .cumprod() - Return cumulative product over a DataFrame or Series axis
buy_hold_return = (1 + data['Daily Return']).cumprod()
strategy_return = (1 + data['Strategy Return']).cumprod()
plt.figure(figsize=(12,6))
plt.plot(buy_hold_return, label='Buy & Hold')
plt.plot(strategy_return, label='SMA Strategy')
plt.legend()
plt.title(f'{ticker} Strategy vs Buy & Hold')
plt.show()