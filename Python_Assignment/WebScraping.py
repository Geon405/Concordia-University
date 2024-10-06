import sys
import sampleScraper

def displayMenu():
    print("\n***** Web Scraping Adventure *****")
    print("1. Display Latest Deals")
    print("2. Analyze Deals by Category")
    print("3. Find Top Stores")
    print("4. Log Deal Information")
    print("5. Exit")

def get_user_choice():
    """
    Get the user's choice from the menu.

    Returns:
    - int: The user's choice.
    """
    choice = input("Enter your choice (1-5): ")
    return int(choice)

def main():
    """
    Main function to control the web scraping adventure program.
    """
    while True:
        displayMenu()
        choice = get_user_choice()
        
        if choice == 1:
            # Call the main function from sampleScraper.py to scrape and display the latest deals
            total_deals, deals_info, _, _ = sampleScraper.main()
            print(f"\nTotal deals found: {total_deals}\n")
            for deal in deals_info:
                print(f"Store: {deal['store']}")
                print(f"Item: {deal['item']}")
                print(f"Votes: {deal['votes']}")
                print(f"Username: {deal['username']}")
                print(f"Timestamp: {deal['timestamp']}")
                print(f"Category: {deal['category']}")
                print(f"Replies: {deal['replies']}")
                print(f"Views: {deal['views']}")
                print(f"URL: {deal['url']}")
                print("-------------------------")

        elif choice == 2:
            _, _, category_counts, _ = sampleScraper.main()
            print("\nDeals by Category:\n")
            for category, count in category_counts.items():
                print(f"{category}: {count} deals")
            print("-------------------------")

        elif choice == 3:
            _, _, _, store_counts = sampleScraper.main()
            num_top_stores = int(input("Enter the number of top stores to display: "))
            
            # Sort the store counts by the number of deals in descending order
            sorted_stores = sorted(store_counts.items(), key=lambda store_count: store_count[1], reverse=True)

            # Select the top stores based on the specified number
            top_stores = sorted_stores[:num_top_stores]
            
            print("\nTop Stores:\n")
            for store, count in top_stores:
                print(f"{store}: {count} deals")
            print("-------------------------")

        elif choice == 4:
            print("\nList of Categories:\n")
            _, _, category_counts, _ = sampleScraper.main()
            categories = list(category_counts.keys())
            for index, category in enumerate(categories, start=1):
                print(f"{index}. {category}")

            selected_category_index = int(input("Enter the number corresponding to the category: ")) - 1
            selected_category = categories[selected_category_index]
            
            deals_info = sampleScraper.get_deals_info_in_category(selected_category)
            if not deals_info:
                print("No deals found in the selected category.")
            else:
                sampleScraper.log_deals(deals_info, selected_category)
                
        elif choice == 5:
            print("Exiting the program. Goodbye!")
            sys.exit()

if __name__ == "__main__":
    main()