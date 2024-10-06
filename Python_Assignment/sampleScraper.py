import requests
from bs4 import BeautifulSoup

def get_store(listing):
    """
    Extracts the store name from the given listing.

    Parameters:
    - listing (BeautifulSoup): The BeautifulSoup object representing a deal listing.

    Returns:
    - str: The extracted store name.
    """
    store_element_retailer = listing.select_one('.topictitle_retailer')
    store_element = listing.select_one('.topictitle')

    if store_element_retailer:
        return store_element_retailer.text.strip()
    elif store_element:
        # Extract store from the square brackets, if available
        store_text = store_element.text.strip()
        return store_text.split(']')[0][1:].strip() if ']' in store_text else store_text
    else:
        return "N/A"
    
def get_votes(listing):
    """
    Extracts the number of votes from the given listing.

    Parameters:
    - listing (BeautifulSoup): The BeautifulSoup object representing a deal listing.

    Returns:
    - str: The extracted number of votes.
    """
    votes_element = listing.select_one('.total_count_selector')
    return votes_element.text.strip() if votes_element else "N/A"

def get_username(listing):
    """
    Extracts the username from the given listing.

    Parameters:
    - listing (BeautifulSoup): The BeautifulSoup object representing a deal listing.

    Returns:
    - str: The extracted username.
    """
    username_element = listing.select_one('.thread_meta_author')
    return username_element.text.strip() if username_element else "N/A"

def get_timestamp(listing):
    """
    Extracts the timestamp from the given listing.

    Parameters:
    - listing (BeautifulSoup): The BeautifulSoup object representing a deal listing.

    Returns:
    - str: The extracted timestamp.
    """
    timestamp_element = listing.select_one('.first-post-time')
    return timestamp_element.text.strip() if timestamp_element else "N/A"

def get_category(listing):
    """
    Extracts the category from the given listing.

    Parameters:
    - listing (BeautifulSoup): The BeautifulSoup object representing a deal listing.

    Returns:
    - str: The extracted category.
    """
    category_element = listing.select_one('.thread_category a')
    return category_element.text.strip() if category_element else "N/A"

def get_replies(listing):
    """
    Extracts the number of replies from the given listing.

    Parameters:
    - listing (BeautifulSoup): The BeautifulSoup object representing a deal listing.

    Returns:
    - str: The extracted number of replies.
    """
    replies_element = listing.select_one('.posts')
    return replies_element.text.strip() if replies_element else "N/A"

def get_views(listing):
    """
    Extracts the number of views from the given listing.

    Parameters:
    - listing (BeautifulSoup): The BeautifulSoup object representing a deal listing.

    Returns:
    - str: The extracted number of views.
    """
    views_element = listing.select_one('.views')
    return views_element.text.strip() if views_element else "N/A"

# Define other functions as needed...

def get_deals_info_in_category(category_name):
    """
    Extracts and returns detailed information about deals within the specified category.

    Parameters:
    - category (str): The category for which to extract deal information.

    Returns:
    - list: A list of dictionaries containing detailed information about deals within the specified category.
    """
    category_to_url = {
    "apparel": "?c=12",
    "automative": "?c=11",
    "beauty & wellness": "?c=10",
    "computer & electronics": "?c=9",
    "entertainment": "?c=8",
    "financial services": "?c=7",
    "groceries": "?c=53",
    "home & garden": "?c=5",
    "kids & babies": "?c=196",
    "restaurants": "?c=6",
    "small business": "?c=4",
    "sports & fitness": "?c=3",
    "travel": "?c=2"
    }

    # Format the category name for use in URL
    formatted_category = category_to_url.get(category_name.lower())

    url = f"https://forums.redflagdeals.com/hot-deals-f9/{formatted_category}/"
    response = requests.get(url)
    response.raise_for_status()
    soup = BeautifulSoup(response.content, "html.parser")
    deals_info = []

    for listing in soup.find_all("li", class_="row topic"):
        store = get_store(listing)
        votes = get_votes(listing)
        username = get_username(listing)
        timestamp = get_timestamp(listing)
        replies = get_replies(listing)
        views = get_views(listing)

        item_element = listing.select_one('.topic_title_link')
        item = item_element.text.strip() if item_element else "N/A"
        url = item_element['href'] if item_element else "N/A"

        deal_info = {
            "store": store,
            "item": item,
            "votes": votes,
            "username": username,
            "timestamp": timestamp,
            "replies": replies,
            "views": views,
            "url": url
        }
        
        deals_info.append(deal_info)

    return deals_info

def log_deals(deals_info, selected_category):
    """
    Logs the deal information to a text file.

    Parameters:
    - deals_info (list): A list of dictionaries containing deal information.
    - selected_category (str): The selected category.
    """
    try:
        with open("log.txt", "w") as file:
            file.write(f"Deals in Category: {selected_category}\n")
            for deal in deals_info:
                file.write(f"URL: {deal['url']}\n\n")
        print(f"\nAll the links have been logged successfully.")
    except IOError as e:
        print(f"An error occurred while writing to the file: {e}")


def main():
    """
    Main function to scrape and display deal information from the RedFlagDeals forum.

    Returns:
    - tuple: A tuple containing the total number of deals found, a list of dictionaries with deal information, a dictionary containing deal counts by category, and a dictionary containing deal counts by store.
    """
    url = "https://forums.redflagdeals.com/"
    response = requests.get("https://forums.redflagdeals.com/hot-deals-f9/")
    response.raise_for_status()
    soup = BeautifulSoup(response.content, "html.parser")

    # Example: Extracting information from HTML elements
    # Base URL
    base_url = "https://forums.redflagdeals.com/"

    total_deals = 0  # Initialize total deals counter
    deals_info = []  # Initialize list to store individual deal information
    category_counts = {}  # Initialize dictionary to store deal counts by category
    store_counts = {}  # Initialize dictionary to store deal counts by store

    for listing in soup.find_all("li", class_="row topic"):
        store = get_store(listing)
        votes = get_votes(listing)
        username = get_username(listing)
        timestamp = get_timestamp(listing)
        category = get_category(listing)
        replies = get_replies(listing)
        views = get_views(listing)

        item_element = listing.select_one('.topic_title_link')
        item = item_element.text.strip() if item_element else "N/A"

        # Extract the URL and prepend the base URL
        url_element = item_element['href'] if item_element else "N/A"
        url = base_url + url_element
        
        # Append deal information to the list
        deals_info.append({
            "store": store,
            "item": item,
            "votes": votes,
            "username": username,
            "timestamp": timestamp,
            "category": category,
            "replies": replies,
            "views": views,
            "url": url
        })

        total_deals += 1

        # Update category counts
        if category in category_counts:
            category_counts[category] += 1
        else:
            category_counts[category] = 1

        # Update store counts
        if store in store_counts:
            store_counts[store] += 1
        else:
            store_counts[store] = 1

    return total_deals, deals_info, category_counts, store_counts

if __name__ == "__main__":
    main()
