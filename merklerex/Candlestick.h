//Midterm assignment

#include <string>
#include <vector>
#include "OrderBookEntry.h"

class Candlestick{
    public:
        std::string time, product, type;
        double high, low, open, close;

        Candlestick(std::string time,
                    std::string product,
                    std::string type,

                    double high,
                    double low,
                    double open,
                    double close);

        //task 1
        void printCandlestick();
        static void printCandlestickTable(std::vector<Candlestick>candles);
        //generate 
        static std::vector<Candlestick> calculateCandlesticks(std::vector<OrderBookEntry> entries);



        //task 2 & 3
        static void printCandlestickPlot(std::vector<Candlestick>candles);

        static void printAdditionalCandlestickPlot(std::vector<Candlestick> candles);
};