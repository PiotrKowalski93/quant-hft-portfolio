import numpy as np
import pandas as pd
import math
import yfinance as yf
import xlsxwriter
from scipy import stats
from statistics import mean

# Equal weight
def calculate_shares(portfolio_size, dataframe, column):
    position_size = portfolio_size/len(dataframe.index)

    for i in range(0, len(dataframe.index)):
        dataframe.loc[i, column] = math.floor(position_size/dataframe.loc[i, 'Price'])
    #print(dataframe)

# Sample momentum strategy
def momentum(stocks):
    # --- Final Data ---
    my_columns = ['Ticker', 'Price', 'One-Year Price Return', '#Shares to Buy']
    final = pd.DataFrame(columns = my_columns)

    lst = []
    # --- Get data from last year ---
    # To optimise use API Batch call
    for stock in stocks['Ticker'][:15]:
        try:
            data = yf.download(stock, period="1y")

            # --- 1Y Change Calculations ---
            # Most of API's have that property on GET
            # To optimise use API Batch call
            price_today = data['Close'].iloc[-1].item() 
            price_year_ago = data['Close'].iloc[0].item() 
            one_year_change = (price_today - price_year_ago) / price_year_ago * 100

            lst.append([stock, price_today, one_year_change, 'N/A'])
        except Exception as e:
            print(f"Error for {stock}: {e}")
    
    final_dataframe = pd.DataFrame(lst, columns=my_columns)
    final_dataframe.sort_values('One-Year Price Return', ascending=False, inplace=True)
    final_dataframe.reset_index(inplace=True)
    calculate_shares(100000, final_dataframe)
    print(final_dataframe)

def calculate_price_change(data):
    first_price = data['Close'].iloc[-1].item() 
    last_price = data['Close'].iloc[0].item() 
    change = (first_price - last_price) / first_price * 100
    return change


stocks = pd.read_csv("./Equal-Weight S&P 500 Screener/sp_500_stocks.csv")

# --- High Quality Momentum ---
hqm_columns = [
    'Ticker',
    'Price',
    'Number of Shares to Buy',
    'One-Year Price Return',
    'One-Year Price Percentile',
    'Six-Months Price Return',
    'Six-Months Price Percentile',
    'Three-Months Price Return',
    'Three-Months Price Percentile',
    'One-Month Price Return',
    'One-Month Price Percentile',
    'HQM Score'
]

hqm_dataframe = pd.DataFrame(columns=hqm_columns)

lst = []
for stock in stocks['Ticker'][:15]:
    try:
        data_1y = yf.download(stock, period="1y")
        data_6mo = yf.download(stock, period="6mo")
        data_3mo = yf.download(stock, period="3mo")
        data_1mo = yf.download(stock, period="1mo")

        change_1y = calculate_price_change(data_1y)
        change_6mo = calculate_price_change(data_6mo)
        change_3mo = calculate_price_change(data_3mo)
        change_1mo = calculate_price_change(data_1mo)

        last_price = data_1mo['Close'].iloc[-1].item()

        lst.append([stock, last_price, 'N/A', change_1y, 'N/A', change_6mo, 'N/A', change_3mo, 'N/A', change_1mo, 'N/A', 'N/A'])
    except Exception as e:
        print(f"{e}")

hqm_dataframe = pd.DataFrame(lst, columns=hqm_columns)

time_periods = [
        'One-Year',
        'Six-Months',
        'Three-Months',
        'One-Month'
    ]

# --- Calculating actual percentiles ---
for row in hqm_dataframe.index:
    for time_period in time_periods:
        change_column = f'{time_period} Price Return'
        percentile_column = f'{time_period} Price Percentile'

        hqm_dataframe.loc[row, percentile_column] = stats.percentileofscore(hqm_dataframe[change_column], 
                                                                                        hqm_dataframe.loc[row, change_column])

# --- Calculating HQM Score --- 
for row in hqm_dataframe.index:
    momentum_percentiles = []
    for time_period in time_periods:
        percentile_column = f'{time_period} Price Percentile'
        momentum_percentiles.append(hqm_dataframe.loc[row, percentile_column])
    hqm_dataframe.loc[row, 'HQM Score'] = mean(momentum_percentiles)

hqm_dataframe.sort_values('HQM Score', ascending=False, inplace=True)
hqm_dataframe = hqm_dataframe[:5]
hqm_dataframe.reset_index(drop=True, inplace=True)

# --- Calculate Number of shares to buy --- 
calculate_shares(1000000, hqm_dataframe, 'Number of Shares to Buy')
print(hqm_dataframe)

# --- Save in excel for Quants ---
writer = pd.ExcelWriter('strategy.xlsx', engine = 'xlsxwriter')
hqm_dataframe.to_excel(writer, 'Recommended trades', index = False)

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
        'num_format': '$0.0000',
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

float_format = writer.book.add_format(
    {
        'num_format': '0.0000',
        'font_color': font_color,
        'bg_color': backgroud_color,
        'border': 1
    }
)

column_formats = {
    'A' : ['Ticker', string_format],
    'B' : ['Price', dollar_format],
    'C' : ['Number of Shares to Buy', int_format],
    'D' : ['One-Year Price Return', int_format],
    'E' : ['One-Year Price Percentile', float_format],
    'F' : ['Six-Months Price Return', float_format],
    'G' : ['Six-Months Price Percentile', float_format],
    'H' : ['Three-Months Price Return', float_format],
    'I' : ['Three-Months Price Percentile', float_format],
    'J' : ['One-Month Price Return', float_format],
    'K' : ['One-Month Price Percentile', float_format],
    'L' : ['HQM Score', float_format]
}

for column in column_formats.keys():
    writer.sheets['Recommended trades'].set_column(f'{column}:{column}', 18, column_formats[column][1])

writer.close()