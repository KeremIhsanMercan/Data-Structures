/*@Author
StudentName :Kerem Ýhsan Mercan
StudentID :150210042
Date :04.12.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;


class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d) {
            id = i;
            salary = s;
            department = d;
        }
        void set_id(int new_id) {
            id = new_id;
        }
        void set_salary(int new_salary) {
            salary = new_salary;
        }
        void set_department(int new_department) {
            department = new_department;
        }
        int get_id() {
            return id;
        }
        int get_salary() {
            return salary;
        }
        int get_department() {
            return department;
        }
        
};

class Node{
    Employee *employee;
    Node *left;
    Node *right;

public:
    Node(Employee *employee) {
        this->employee = employee;
        left = NULL;
        right = NULL;
    }
    void set_employee(Employee *new_emp) {
        employee = new_emp;
    }
    void set_left(Node *node) {
        left = node;
    }
    void set_right(Node *node) {
        right = node;
    }
    Employee* get_emp() {
        return employee;
    }
    Node* get_left() {
        return left;
    }
    Node* get_right() {
        return right;
    }
    bool isExternal() {
        if (left == NULL && right == NULL) {
            return true;
        } else {
            return false;
        }
        
    }
};

class BinaryTree{
    private:
        Node *root;
        int getHeight_rec(Node* current);
        void print_each_node_preorder(Node* current);
        void print_each_node_inorder(Node* current, ofstream &output_file);
    public:
        BinaryTree() {
            root = NULL;
        }
        void insert(Employee* employee);
        void remove(int id);
        Employee *search(int id);
        int getHeight();
        void printToFile(ofstream &output_file);
        void printToConsole();
};

void print_height(BinaryTree &bt) {
    cout << "H " << bt.getHeight() << "\n";
}

void print_nodes_to_console(BinaryTree &bt) {
    cout << "P\n"; bt.printToConsole();
}


void BinaryTree::insert(Employee* employee_to_insert) {

    Employee* new_emp_to_insert = new Employee(employee_to_insert->get_id(), employee_to_insert->get_salary(), employee_to_insert->get_department());
    Node* new_node = new Node(new_emp_to_insert);

    if (root == NULL) {
        root = new_node;
        return;
    }

    Node* next = root;
    Node* current = NULL;

    while (next != NULL) {
        current = next;

        if (next->get_emp()->get_id() > employee_to_insert->get_id()) {
            next = next->get_left();
        } else {
            next = next->get_right();
        }
    }

    if (current->get_emp()->get_id() > employee_to_insert->get_id()) {
        current->set_left(new_node);
    } else {
        current->set_right(new_node);
    }

}

void BinaryTree::remove(int id) {

    if (root == NULL) {
        cout << "ERROR: There is no Employee" << "\n";
        return;
    }

    Node* current = root;
    Node* current_p = NULL;
    
    while (current != NULL) {

        if (current->get_emp()->get_id() > id) {
            current_p = current;
            current = current->get_left();
        } else if (current->get_emp()->get_id() < id) {
            current_p = current;
            current = current->get_right();
        } else {    // valid removal condition

            if (current->isExternal()) {
                if (current_p == NULL) { // current is root
                    root = NULL;
                } else if (current_p->get_left() == current) {
                    current_p->set_left(NULL);
                } else {
                    current_p->set_right(NULL);
                }
                
                delete current->get_emp();
                delete current;
                return;
            }

            if (current->get_left() == NULL) {
                if (current_p == NULL) { // current is root
                    root = current->get_right();
                } else if (current_p->get_left() == current) {
                    current_p->set_left(current->get_right());
                } else {
                    current_p->set_right(current->get_right());
                }

                delete current->get_emp();
                delete current;
                return;
            }

            if (current->get_right() == NULL) {
                if (current_p == NULL) { // current is root
                    root = current->get_left();
                } else if (current_p->get_left() == current) {
                    current_p->set_left(current->get_left());
                } else {
                    current_p->set_right(current->get_left());
                }

                delete current->get_emp();
                delete current;
                return;
            }
            
            /*remaining operations will be applied only when current has 2 children*/
        
            Node* node_to_replace = current->get_right();
            Node* node_to_replace_p = current;

            while (node_to_replace->get_left() != NULL) {
                node_to_replace_p = node_to_replace;
                node_to_replace = node_to_replace->get_left();
            }
            
            if (node_to_replace_p == current) {
                node_to_replace_p->set_right(node_to_replace->get_right());
            } else {
                node_to_replace_p->set_left(node_to_replace->get_right());
            }
            delete current->get_emp();
            current->set_employee(node_to_replace->get_emp());

            delete node_to_replace;
            return;
        }
    }
    
    cout << "ERROR: An invalid ID to delete" << "\n";
    return;

}

Employee* BinaryTree::search(int id) { // returns NULL if not found

    if (root == NULL) {
        return NULL;
    }

    Node* current = root;
    while (current != NULL) {

        if (current->get_emp()->get_id() > id) {
            current = current->get_left();
        } else if (current->get_emp()->get_id() < id) {
            current = current->get_right();
        } else {
            return current->get_emp();
        }
    }
    
    return NULL;
}

int BinaryTree::getHeight() {
    return getHeight_rec(root);
}

int BinaryTree::getHeight_rec(Node* current) { // root is given initially
    if (current == NULL) {
        return 0;
    }

    return max(getHeight_rec(current->get_right()), getHeight_rec(current->get_left())) + 1;     
}

void BinaryTree::printToFile(ofstream &output_file) {

    print_each_node_inorder(root, output_file);

}

void BinaryTree::print_each_node_inorder(Node* current, ofstream &output_file) {

    if (current == NULL) {
        return;
    }

    print_each_node_inorder(current->get_left(), output_file);
    
    output_file << current->get_emp()->get_id() << ";" << current->get_emp()->get_salary() << ";" << current->get_emp()->get_department() << "\n";

    print_each_node_inorder(current->get_right(), output_file);

}

void BinaryTree::printToConsole() {

    print_each_node_preorder(root);
}

void BinaryTree::print_each_node_preorder(Node* current) {

    if (current == NULL) {
        return;
    }
    
    cout << current->get_emp()->get_id() << ";" << current->get_emp()->get_salary() << ";" << current->get_emp()->get_department() << "\n";

    print_each_node_preorder(current->get_left());
    print_each_node_preorder(current->get_right());

}

int main(int argc, char **argv) {

    // clock_t start = clock();

    if (argc != 3) {
        cout << argc << " file provided! It is not the right amount." << "\n";
        return 0;
    }

    BinaryTree bt;

    fstream mini_data_set(argv[1]);

    string one_line;
    string id_temp, salary_temp, department_temp;
    string first_line;
    bool is_first_line_skipped = false;
    int highest_id;

    while (getline(mini_data_set, one_line, '\n')) {
        if (!is_first_line_skipped) {
            first_line = one_line;
            is_first_line_skipped = true;
            continue;
        }
        
        stringstream one_line_split(one_line);

        getline(one_line_split, id_temp, ';');
        getline(one_line_split, salary_temp, ';');
        getline(one_line_split, department_temp, '\n');

        Employee temp_emp(stoi(id_temp), stoi(salary_temp), stoi(department_temp));
        bt.insert(&temp_emp);

        highest_id = max(highest_id, stoi(id_temp));

    }
    mini_data_set.close();


    fstream operations_file_var(argv[2]);
    string operation_name;
    
    while (getline(operations_file_var, one_line, '\n')) {

        stringstream one_line_split(one_line);
        getline(one_line_split, operation_name, ';');        

        if (operation_name[0] == 'P') {
            print_nodes_to_console(bt);
            
        } else if (operation_name[0] == 'H') {
            print_height(bt);
            
        } else if (operation_name == "ADD") {
            string new_emp_salary, new_emp_department;
            getline(one_line_split, new_emp_salary, ';');
            getline(one_line_split, new_emp_department, '\n');

            Employee temp_emp(++highest_id, stoi(new_emp_salary), stoi(new_emp_department));
            bt.insert(&temp_emp);

        } else if (operation_name == "UPDATE") {
            string id_to_update, updated_salary, updated_department;
            getline(one_line_split, id_to_update, ';');
            getline(one_line_split, updated_salary, ';');
            getline(one_line_split, updated_department, '\n');

            Employee* employee_to_update = bt.search(stoi(id_to_update));

            if (employee_to_update == NULL) {
                cout << "ERROR: An invalid ID to update" << "\n";
                continue;
            }
            
            employee_to_update->set_salary(stoi(updated_salary));
            employee_to_update->set_department(stoi(updated_department));

        } else if (operation_name == "DELETE") {
            string id_to_delete;
            getline(one_line_split, id_to_delete, '\n');

            bt.remove(stoi(id_to_delete));

        }
    }
    operations_file_var.close();

    ofstream output_file_var("binary_tree_solution.csv");
    output_file_var << first_line << "\n";
    bt.printToFile(output_file_var);
    output_file_var.close();

    // clock_t end = clock();
    // cout << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";

    return 0;
}
