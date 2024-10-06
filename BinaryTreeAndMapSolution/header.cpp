/*@Author
StudentName :Kerem Ýhsan Mercan
StudentID :150210042
Date :04.12.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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