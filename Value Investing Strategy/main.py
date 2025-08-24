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
data_columns = [
    'Ticker', 
    'Price', 
    'Trailing (P/E)', 
    'T P/E Per.',
    'Forward (P/E)', 
    'F P/E Per.',
    'Price to Book (P/B)', 
    'P/B Per.',
    'Price to Sales (P/S)',
    'P/S Per.',
    'EV/EBITDA', 
    'EV/EBITDA Per.',
    'EV/GP', 
    'EV/GP Per.',
    'RV Score',
    'Shares to Buy']

lst = []
for stock in stocks['Ticker'][:25]:
    ticker = yf.Ticker(stock)
    info = ticker.info

    #Enterprise Value (EV) = marketCap + totalDebt – cash
    #EV/GP = Enterprise Value ÷ Gross Profit.
    ev = info.get("enterpriseValue")
    gp = info.get("grossProfits")
    if ev and gp and gp != 0:
        ev_gp = ev / gp
    else:
        ev_gp = None

    lst.append([
        stock, 
        info.get("currentPrice"), 
        info.get("trailingPE"), 
        'N/A',
        info.get("forwardPE"), 
        'N/A',
        info.get("priceToBook"),
        'N/A',
        info.get("priceToSalesTrailing12Months"),
        'N/A',
        info.get("enterpriseToEbitda"),
        'N/A',
        ev_gp,
        'N/A',
        'N/A',
        'N/A'])

final_data = pd.DataFrame(lst, columns = data_columns)
final_data.sort_values('Trailing (P/E)', ascending=False, inplace=True)
final_data = final_data[final_data['Trailing (P/E)'] > 0]
final_data.reset_index(inplace=True)
final_data.drop('index', axis=1, inplace=True)

print(final_data)

# Calculate percentyles
metrics = {
    'Trailing (P/E)' : 'T P/E Per.',
    'Forward (P/E)' : 'F P/E Per.',
    'Price to Book (P/B)' : 'P/B Per.',
    'Price to Sales (P/S)' : 'P/S Per.',
    'EV/EBITDA' : 'EV/EBITDA Per.',
    'EV/GP' : 'EV/GP Per.'
    }

for metric in metrics.keys():
    for row in final_data.index:
        final_data.loc[row, metrics[metric]] = stats.percentileofscore(final_data[metric], final_data.loc[row, metric])


# Calculate Robust Value 
for row in final_data.index:
    sum = 0
    for metric in metrics.keys():
        sum += final_data.loc[row, metrics[metric]]
    rv = sum/len(metrics)
    final_data.loc[row, 'RV Score'] = rv

final_data.sort_values('RV Score', ascending=False, inplace=True)
final_data.reset_index(inplace=True)
final_data.drop('index', axis=1, inplace=True)

final_data = final_data[:10]
calculate_shares(100000, final_data, 'Shares to Buy')

print(final_data)