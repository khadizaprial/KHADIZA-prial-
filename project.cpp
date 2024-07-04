
#include <bits/stdc++.h>
using namespace std;

unordered_map<string, string> readUserData(const string& filename) {
    unordered_map<string, string> ok;
    fstream infile(filename, ios::in);
    string username, password;
    if (infile.is_open()) {
        while (infile >> username >> password) {
            ok[username] = password;
        }
        infile.close();
    } else {
        cout << "Error file for reading!" << endl;
    }
    return ok;
}

void writeUserData(const string& filename, const unordered_map<string, string>& ok) {
    fstream outfile(filename, ios::out);
    if (outfile.is_open()) {
        for (const auto& pair : ok) {
            outfile << pair.first << " " << pair.second << "\n";
        }
        outfile.close();
    } else {
        cout << "Error  file for writing!" << endl;
    }
}

struct Product {
    char name[50];
    double price;
    int quantity;
    struct Product* next;
};

void addProduct(struct Product** head, const char* name, double price, int quantity) {
    struct Product* newProduct = (struct Product*)malloc(sizeof(struct Product));
    if (newProduct == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(newProduct->name, name);
    newProduct->price = price;
    newProduct->quantity = quantity;
    newProduct->next = NULL;

    if (*head == NULL) {
        *head = newProduct;
    } else {
        struct Product* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newProduct;
    }

    printf("Product added successfully.\n");
}

void displayProducts(struct Product* head) {
    if (head == NULL) {
        printf("No products to display.\n");
        return;
    }

    printf("Products in the store:\n");
    struct Product* current = head;
    while (current != NULL) {
        printf("Product Name: %s, Price: %.2f, Quantity: %d\n",
               current->name, current->price, current->quantity);
        current = current->next;
    }
}

void searchProduct(struct Product* head, const char* searchName) {
    if (head == NULL) {
        printf("No products to search.\n");
        return;
    }

    struct Product* current = head;
    int found = 0;

    printf("Search results for products with name '%s':\n", searchName);
    while (current != NULL) {
        if (strcmp(current->name, searchName) == 0) {
            printf("Product Name: %s, Price: %.2f, Quantity: %d\n",
                current->name, current->price, current->quantity);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No matching products found.\n");
    }
}

void saveProducts(const string& filename, struct Product* head) {
    fstream outfile(filename, ios::out);
    if (outfile.is_open()) {
        struct Product* current = head;
        while (current != NULL) {
            outfile << current->name << " " << current->price << " " << current->quantity << "\n";
            current = current->next;
        }
        outfile.close();
    } else {
        cout << "Error opening file for writing!" << endl;
    }
}

struct Product* loadProducts(const string& filename) {
    struct Product* head = NULL;
    fstream infile(filename, ios::in);
    if (infile.is_open()) {
        while (!infile.eof()) {
            struct Product* newProduct = (struct Product*)malloc(sizeof(struct Product));
            if (newProduct == NULL) {
                printf("Memory allocation failed.\n");
                return head;
            }
            infile >> newProduct->name >> newProduct->price >> newProduct->quantity;
            newProduct->next = NULL;
            if (head == NULL) {
                head = newProduct;
            } else {
                struct Product* current = head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newProduct;
            }
        }
        infile.close();
    } else {
        cout << "Error opening file for reading!" << endl;
    }
    return head;
}

void productMenu() {
    struct Product* store = loadProducts("products.txt");
    int choice;

    do {
        printf("\nStore Management System\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Search Product by Name\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int quantity;
                char name[50];
                double price;

                printf("Enter Product Name: ");
                scanf("%s", name);
                printf("Enter Price: ");
                scanf("%lf", &price);
                printf("Enter Quantity: ");
                scanf("%d", &quantity);

                addProduct(&store, name, price, quantity);
                saveProducts("products.txt", store);
                break;
            }
            case 2:
                displayProducts(store);
                break;
            case 3: {
                char searchName[50];
                printf("Enter Product Name to search: ");
                scanf("%s", searchName);
                searchProduct(store, searchName);
                break;
            }
            case 4:
                printf("Exiting...\n");
                saveProducts("products.txt", store);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

int main() {
    string filename = "userdata.txt";
    unordered_map<string, string> ok = readUserData(filename);

    int attempt_count = 0;

    while (true) {
        cout << "1. Login\n2. Register\n3. Exit\nEnter option: ";
        string option;
        cin >> option;

        if (option == "1") {
            if (attempt_count >= 5) {
                cout << "You have tried 5 times and you cannot try anymore.\n";
                break;
            }

            cout << "Enter username: ";
            string username;
            cin >> username;

            if (ok.find(username) == ok.end()) {
                cout << "Invalid username\n";
                attempt_count++;
                if (attempt_count >= 3 && attempt_count < 5) {
                    cout << "Attempt " << attempt_count << " failed. " << 5 - attempt_count << " attempt(s) left.\n";
                }
                continue;
            }

            cout << "Enter password: ";
            string password;
            cin >> password;

            if (ok[username] == password) {
                cout << "Login successful\n";
                productMenu();
                break;
            } else {
                cout << "Invalid password\n";
                attempt_count++;
                if (attempt_count >= 3 && attempt_count < 5) {
                    cout << "Attempt " << attempt_count << " failed. " << 5 - attempt_count << " attempt(s) left.\n";
                }
            }
        } else if (option == "2") {
            cout << "Enter new username: ";
            string username;
            cin >> username;
            cout << "Enter new password: ";
            string password;
            cin >> password;

            if (ok.find(username) == ok.end()) {
                ok[username] = password;
                writeUserData(filename, ok);
                cout << "Registration successful\n";
            } else {
                cout << "Username already exists\n";
            }
        } else if (option == "3") {
            cout << "Exiting\n";
            break;
        } else {
            cout << "Invalid option, please try again\n";
        }
    }
    return 0;
}
