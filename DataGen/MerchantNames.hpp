#ifndef MERCHANTNAMES_HPP
#define MERCHANTNAMES_HPP
/*
    static lookup tables for entries into business list:20*20*20 gets us 8000 businesses
*/
#include <string>

//to get iterators to these, use <iterator> and std::begin(arrayName), then we can use std::methods
namespace MerchantGenNames{
//first word is owner name, current size 45, all need to be 45
static std::string owners[]{
    "Arnold's",
    "Andrew's",
    "Ahmed's",
    "Aiesha's",
    "Ansem's",
    "Bailey's",
    "Barney's",
    "Basil's",
    "Billy's",
    "Bruce's",
    "Bryce's",
    "Cathy's",
    "Cedric's",
    "Charley's",
    "Cynthia's",
    "Corrine's",
    "Danny's",
    "Devonte's",
    "Diego's",
    "Daphne's",
    "Dylan's",
    "Endo's",
    "Emily's",
    "Eric's",
    "Francine's",
    "Fred's",
    "Frank's",
    "George's", 
    "Lloyd's",
    "Clarence's",
    "Clementine's",
    "Jeffy's",
    "Gemini's",
    "Harold's",
    "Harris's",
    "Mark's",
    "Micheal's",
    "Mikheal's",
    "Stacie's",
    "Chad's",
    "Enrique's",
    "Stephan's",
    "Steven's",
    "Juliet's",
    "Jasmine's",
    "Julius's"
};

//second word is the product or service, current size: 50
static std::string goods[] = {
    "Apple",
    "Answer",
    "Bottle",
    "Stamp",
    "Lobster",
    "Tile",
    "Wrench",
    "Door",
    "Computer",
    "Movie",
    "Speaker",
    "Electrical",
    "Pottery",
    "Candle",
    "Shoe",
    "Frame",
    "Lumber",
    "Dessert",
    "Candy",
    "Book",
    "Fan",
    "Stationary",
    "Camera", 
    "Grocery", 
    "Tire",
    "Plumbing",
    "Automobile",
    "Car",
    "Truck",
    "Limosine",
    "Jeep",
    "Crepe",
    "Bread",
    "Cake",
    "Pie",
    "Bar-Supply",
    "Hot-Tub",
    "Smartphone",
    "Gaming",
    "Newspaper",
    "Pen",
    "Pencil",
    "Thermometer",
    "Robot",
    "Multivitamin",
    "Dresser",
    "Suit",
    "Tuxedo",
    "Wallet",
    "Job",
    "Window"
};

//last word is the type of establishment, current size: 50
static std::string places[] = {
    "Abbatoir",
    "Hangout",
    "Location",
    "Specialist",
    "Practice",
    "and-Things",
    "Consignment",
    "Hole",
    "Haunt",
    "Collection",
    "Position",
    "Dealer",
    "Trade",
    "Corporation",
    "Merchandiser",
    "Institute",
    "Outlet",
    "Biz",
    "Emporium",
    "Palace",
    "Place",
    "Store",
    "Stop",
    "Kingdom",
    "Barn",
    "Spot",
    "Shop",
    "Market",
    "Warehouse",
    "Dive",
    "Mart",
    "Center",
    "Business",
    "Company",
    "Enterprise"
    "Firm",
    "Establishment",
    "House",
    "Outfit",
    "Vocation",
    "Lender",
    "Specialty",
    "Crafter",
    "Organization",
    "Mom-and-Pop",
    "Syndicate",
    "Megacorp",
    "Partnership",
    "Trust",
    "Setup",
    "Trafficker"
};

}

//just make the short codes and the categories random but assign them consistently, it's fine. Current size 10
static std::string MCCs[]={
    "5937", //antiques
    "5971", // art
    "5462", //bakery
    "5942", //books
    "5441", //candy
    "5734", //computers
    "5311", // department stores
    "5310", // discount
    "5732", // electronics
    "5651" //clothing
};
#endif