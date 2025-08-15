import numpy as np
import pandas as pd
import requests
import xlsxwriter
import math
import yfinance as yf

import os
print("Path at terminal when executing this file")
print(os.getcwd() + "\n")

## Loading API Key
from config import IEX_CLOUD_API_TOKEN

## Importing list of stocs
stocks = pd.read_csv("./Equal-Weight S&P 500 Screener/sp_500_stocks.csv")
# print(stocks)
# for stock in stocks['Ticker']:
#     print(stock)

## Making API Call for all stock
dataColumns = ['Symbol', 'Stock Price', 'Market Cap', 'Number of shares to Buy']
final_dataframe = pd.DataFrame(columns = dataColumns)

lst = []
for stock in stocks['Ticker'][:5]:
    try:
        ticker = yf.Ticker(stock)
        info = ticker.info

        if not info or info.get("currentPrice") is None:
            raise ValueError("Brak danych dla ticker'a")

        lst.append([stock, info.get("currentPrice"), info.get("marketCap"), 'N/A'])

    except(requests.exceptions.HTTPError, ValueError) as e:
        print(f"Error for {stock}: {e}")
        lst.append([stock, None, None, 'N/A'])

final_dataframe = pd.DataFrame(lst, columns=dataColumns)
print(final_dataframe)

## Calculating shares to buy
portfolio_size = 1000000
position_size = portfolio_size/len(final_dataframe.index)
print(position_size)

for i in range(0, len(final_dataframe.index)):
    final_dataframe.loc[i, 'Number of shares to Buy'] = math.floor(position_size/final_dataframe.loc[i, 'Stock Price'])

print(final_dataframe)

## Save in excel for Quants
writer = pd.ExcelWriter('strategy.xlsx', engine = 'xlsxwriter')
final_dataframe.to_excel(writer, 'Recommended trades', index = False)

### Formatting for excel file
backgroud_color = '#0a0a23'
font_color = '#ffffff'

string_format = writer.book.add_format(
    {
        'font_color': font_color,
        'bg_color': backgroud_color,
        'border': 1
    }
)

dollar_format = writer.book.add_format(
    {
        'num_format': '$0.00',
        'font_color': font_color,
        'bg_color': backgroud_color,
        'border': 1
    }
)

int_format = writer.book.add_format(
    {
        'num_format': '0',
        'font_color': font_color,
        'bg_color': backgroud_color,
        'border': 1
    }
)

#writer.sheets['Recommended Trades'].set_column('A:A',   #This tells the method to apply the format to column A
#                     18,                                #This tells the method to apply a column width of 18 pixels
#                     string_format)                     #This applies the format 'string_template' to the column

#writer.sheets['Recommended Trades'].set_column('B:B', 18, dollar_format)
#writer.sheets['Recommended Trades'].set_column('C:C', 18, int_format)
#writer.sheets['Recommended Trades'].set_column('D:D', 18, int_format)
#writer.close()

column_formats = {
    'A' : ['Symbol', string_format],
    'B' : ['Stock Price', dollar_format],
    'C' : ['Market Cap', int_format],
    'D' : ['Number of shares to Buy', int_format]
}

for column in column_formats.keys():
    writer.sheets['Recommended trades'].set_column(f'{column}:{column}', 18, column_formats[column][1])
writer.close()