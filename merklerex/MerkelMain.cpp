#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
 

MerkelMain::MerkelMain()
{
}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;


    //-----------------
    // start midterm
    std::cout << "6: Print candlestick data " << std::endl;
    std::cout << "7: Print candlestick plot " << std::endl;
    std::cout << "8: Print candlestick plot with additional data " << std::endl;

    // end midterm
    //-----------------


    // 9 continue
    std::cout << "9: Continue " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }
    // }
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask);
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid);
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-9" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1)
    {
        printHelp();
    }
    if (userOption == 2)
    {
        printMarketStats();
    }
    if (userOption == 3)
    {
        enterAsk();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    //-----------------
    //midterm start
    if (userOption == 6)
    {
        printCandlestickData();
    }
    if (userOption == 7)
    {
        printCandlestickPlot();
    }
    if (userOption == 8)
    {
        printAdditionalCandlestickPlot();
    }
    //midterm end
    //-----------------

    //option 9 continue
    if (userOption == 9)
    {
        gotoNextTimeframe();
    }
}

//----------------------------------------------------------------
    //midterm start

    //Task 1
    void MerkelMain::printCandlestickData(){
        //std::cout << "MerkelMain::printCandlestickData" << std::endl;
        std::vector <OrderBookEntry> orders = orderBook.getOrders(OrderBookType::bid, "ETH/BTC");
        std::vector <Candlestick> candles = Candlestick::calculateCandlesticks(orders);
        Candlestick::printCandlestickTable(candles);
        
        //realtime test
        // candles.push_back(Candlestick{"time1", "product1", "type1", 1, 2, 3, 4});
        // candles.push_back(Candlestick{"time2", "product2", "type2", 1, 2, 3, 4});
    }


    //Task 2
    void MerkelMain::printCandlestickPlot(){
        //std::cout << "MerkelMain::printCandlestickPlot" << std::endl;
        std::vector <OrderBookEntry> orders = orderBook.getOrders(OrderBookType::bid, "ETH/BTC");
        std::vector <Candlestick> candles = Candlestick::calculateCandlesticks(orders);
        Candlestick::printCandlestickPlot(candles);

    }


    //Task 3
    void MerkelMain::printAdditionalCandlestickPlot(){
        //std::cout << "MerkelMain::printAdditionalCandlestickPlot" << std::endl;
       std::vector<Candlestick> candles = CSVReader::readCandlestickCSV("BTC-USD.csv");          
       //other trading data source: BTC-USD.csv
       // https://sg.finance.yahoo.com/quote/BTC-USD/history?period1=1672790400&period2=1704326400&interval=1d&filter=history&frequency=1d&includeAdjustedClose=true

       for(Candlestick candle: candles){
        std::cout << candle.time<< "\t"
                  << candle.high<< "\t"
                  << candle.low<< "\t"
                  << candle.open<< "\t"
                  << candle.close<< "\t"
                  <<std::endl;
       }
       
        Candlestick::printCandlestickPlot(candles);
    
    }

    //midterm end
//----------------------------------------------------------------
