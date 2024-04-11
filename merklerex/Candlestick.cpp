//Midterm assignment

#include "Candlestick.h"
#include <iostream>
#include <map>


Candlestick::Candlestick(
                            std::string time,
                            std::string product,
                            std::string type,

                            double high,
                            double low,
                            double open,
                            double close):
        time(time),
        product(product),
        type(type),

        high(high),
        low(low),
        open(open),
        close(close){}


//----------------------------------------------------------------
//Task 1
void Candlestick::printCandlestick(){
    //std::cout << "Candlestick::printCandlestick" << std::endl;
    std::cout << time << "\t"
              << product << "\t"
              << type << "\t"

              << high << "\t"
              << low << "\t"
              << open << "\t"
              << close << "\t"
              << std::endl;
}

void Candlestick::printCandlestickTable(std::vector<Candlestick> candles){
    //std::cout << "Candlestick::printCandlestickTable" << std::endl;
    std::cout << "time" << "\t"
              << "product" << "\t"
              << "type" << "\t"

              << "high" << "\t"
              << "low" << "\t"
              << "open" << "\t"
              << "close" << "\t"
              << std::endl;

    //loop through all data input to print in format
    for(Candlestick &candle : candles){
        candle.printCandlestick();
    }
}

std::vector<Candlestick> Candlestick::calculateCandlesticks(std::vector<OrderBookEntry> entries){
    std::vector<Candlestick> candles;
    std::string product{entries[0].product},
                time{entries[0].timestamp},
                type{OrderBookEntry::obtToString(entries[0].orderType)};
    double high = entries[0].price, low = entries[0].price, open = entries[0].price, close;

    for(OrderBookEntry &entry:entries){
        if(time != entry.timestamp){
            close = entries[entries.size()-1].price;
            candles.push_back(Candlestick{time, product, type, high, low, open, close});
            time = entry.timestamp;
            product = entry.product;
            type = OrderBookEntry::obtToString(entry.orderType);
            open = entry.price;
            high = low = entry.price;
        }
        else{
            high = std::max(high, entry.price);
            low = std::min(low, entry.price);
        }
    }
    // Add the last candlestick
    close = entries[entries.size()-1].price;
    candles.push_back(Candlestick{time, product, type, high, low, open, close});

    return candles;
}

//----------------------------------------------------------------
//Task 2
void Candlestick::printCandlestickPlot(std::vector<Candlestick> candles){
    //std::cout << "Candlestick::printCandlestickPlot" <<std::endl;
    //printing the candles in correct timestamp
    // for(Candlestick &candle : candles){
    //     std::cout <<candle.time <<std::endl;
    // }

    //y range
    double highest = candles[0].high;
    double lowest = candles[0].low;
    for(Candlestick candle: candles){
        highest = std::max(highest, candle.high);
        lowest = std::min(lowest, candle.low);
    }

    //difference
    int steps =100;
    double yRange = highest-lowest;
    double stepSize = yRange/steps;
    for(double y = highest; y>lowest; y-=stepSize){
        std::cout << y  << "\t";

        //candle sizes
        int maxSize = 10;
        if(maxSize > candles.size()){
            maxSize = candles.size();
        }

        //draw candles
        for(int i = 0; i<maxSize; i++){
            Candlestick candle = candles[i];
            if(y>candle.high || y<candle.low){
                std::cout <<"    ";
            }
            else if((y<candle.high && y>candle.open && y>candle.close)||(y>candle.low && y<candle.open && y<candle.close)){
                std::cout <<"  |  ";
            }
            else{
                if(candle.open > candle.close){
                    std::cout << " --- ";

                }
                else{
                    std::cout << " +++ ";
                }
            }
        }

        std::cout << std::endl;
    }

}

//Task 3
//merkelmain.cpp
void Candlestick::printAdditionalCandlestickPlot(std::vector<Candlestick> candles){
    
}