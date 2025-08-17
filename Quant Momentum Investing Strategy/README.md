**Momentum investing** means investing in the stocks that have increased in price the most.

For this project, we're going to build an investing strategy that selects the 50 stocks with the highest price momentum. From there, we will calculate recommended trades for an equal-weight portfolio of these 50 stocks.

Real-world quantitative investment firms differentiate between "high quality" and "low quality" momentum stocks:

High-quality momentum stocks show "slow and steady" outperformance over long periods of time
Low-quality momentum stocks might not show any momentum for a long time, and then surge upwards.
The reason why high-quality momentum stocks are preferred is because low-quality momentum can often be cause by short-term news that is unlikely to be repeated in the future (such as an FDA approval for a biotechnology company).

To identify high-quality momentum, you can build strategy that selects stocks from the highest percentiles of:

- 1-month price returns
- 3-month price returns
- 6-month price returns
- 1-year price returns

**The HQM Score** will be the arithmetic mean of the 4 momentum percentile scores.