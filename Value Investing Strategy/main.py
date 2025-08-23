import numpy as np
import pandas as pd
import yfinance as yf
import xlsxwriter
import math
from scipy import stats
import yfinance as yf

def calculate_shares(portfolio_size, dataframe, column):
    position_size = portfolio_size/len(dataframe.index)

    for i in range(0, len(dataframe.index)):
        dataframe.loc[i, column] = math.floor(position_size/dataframe.loc[i, 'Price'])

## Importing list of stocs
stocks = pd.read_csv("./Equal-Weight S&P 500 Screener/sp_500_stocks.csv")
data_columns = ['Ticker', 'Price', 'Trailing P/E', 'Forward P/E', 'Shares to Buy']

lst = []
for stock in stocks['Ticker'][:25]:
    ticker = yf.Ticker(stock)
    info = ticker.info
    trailing_pe = info.get("trailingPE")
    forward_pe = info.get("forwardPE")
    price = info.get("currentPrice")

    lst.append([stock, price, trailing_pe, forward_pe, 'N/A'])

final_data = pd.DataFrame(lst, columns = data_columns)
final_data.sort_values('Trailing P/E', ascending=False, inplace=True)
final_data = final_data[final_data['Trailing P/E'] > 0]
final_data.reset_index(inplace=True)
final_data.drop('index', axis=1, inplace=True)

calculate_shares(100000, final_data, 'Shares to Buy')

print(final_data)

