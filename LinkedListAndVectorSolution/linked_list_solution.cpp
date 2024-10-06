/* @Author
Student Name : Kerem Ýhsan Mercan
Student ID : 150210042
Date : 01.11.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

class EmployeeNode {
    private:
        int id;
        int salary;
        int department;
        EmployeeNode* next_ptr;

        friend class EmployeeList;
};

class EmployeeList {
    private:
        EmployeeNode* head_ptr;
    public:
        EmployeeList();
        void add_to_front(int id, int salary, int department);
        void delete_from_front();
        void reverse_the_list();
        int get_id(int index);
        int get_salary(int index);
        int get_department(int index);
        EmployeeNode *update_the_node(int id, int updated_salary, int updated_department);
        EmployeeNode *delete_the_node(int id);
};


int employee_count = 0;

int main(int argc, char **argv) {

    // clock_t start = clock();

    if (argc != 3) {
        cout << argc << " file provided! It is not the right amount." << endl;
        return 0;
    }

    EmployeeList emp_list;

    fstream hw2(argv[1]);

    string one_line;
    string id_temp, salary_temp, department_temp;
    string first_line;

    bool is_first_line_skipped = false;
    while (getline(hw2, one_line, '\n')) {
        if (!is_first_line_skipped)
        {
            first_line = one_line;
            is_first_line_skipped = true;
            continue;
        }
        
        stringstream one_line_split(one_line);

        getline(one_line_split, id_temp, ';');
        getline(one_line_split, salary_temp, ';');
        getline(one_line_split, department_temp, '\n');

        emp_list.add_to_front(stoi(id_temp), stoi(salary_temp), stoi(department_temp));
    }

    hw2.close();

    int highest_id = emp_list.get_id(0);

    fstream operations_file_var(argv[2]);
    string operation_name;
    
    while (getline(operations_file_var, one_line, '\n')) {

        stringstream one_line_split(one_line);

        getline(one_line_split, operation_name, ';');

        if (operation_name == "ADD")
        {
            string new_emp_salary, new_emp_department;
            getline(one_line_split, new_emp_salary, ';');
            getline(one_line_split, new_emp_department, '\n');

            emp_list.add_to_front(++highest_id, stoi(new_emp_salary), stoi(new_emp_department));

        } else if (operation_name == "UPDATE")
        {
            string id_to_update, updated_salary, updated_department;
            getline(one_line_split, id_to_update, ';');
            getline(one_line_split, updated_salary, ';');
            getline(one_line_split, updated_department, '\n');

            if (emp_list.update_the_node(stoi(id_to_update), stoi(updated_salary), stoi(updated_department)) == NULL)
            {
                cout << "ERROR: An invalid ID to update";
                continue;
            }

            
        } else if (operation_name == "DELETE")
        {
            string id_to_delete;
            getline(one_line_split, id_to_delete, '\n');

            if (employee_count <= 0)
            {
                cout << "ERROR: There is no Employee";
                continue;
            }

            if (emp_list.delete_the_node(stoi(id_to_delete)) == NULL)
            {
                cout << "ERROR: An invalid ID to delete";
                continue;
            }
        }
    }
    operations_file_var.close();

    ofstream solution_file("linked_list_solution.csv");
    emp_list.reverse_the_list();

    for (int i = 0; i < employee_count; i++) {

        if (i == 0)
        {
            solution_file << first_line;
        }

        solution_file << emp_list.get_id(0) << ";" << emp_list.get_salary(0) << ";" << emp_list.get_department(0) << endl;
        emp_list.delete_from_front();
    }

    solution_file.close();
    
    

    // clock_t end = clock();
    // cout << "execution time of all program: " << (double)(end-start)*1000 / CLOCKS_PER_SEC << " miliseconds!" << endl;
    
    return 0;
}

EmployeeList::EmployeeList() {
    head_ptr = NULL;
}

void EmployeeList::add_to_front(int id, int salary, int department) {
    EmployeeNode* new_emp = new EmployeeNode;

    new_emp->id = id;
    new_emp->salary = salary;
    new_emp->department = department;
    new_emp->next_ptr = head_ptr;

    head_ptr = new_emp;
    employee_count++;
}

void EmployeeList::delete_from_front() {

    EmployeeNode *node_to_delete = head_ptr;
    head_ptr = node_to_delete->next_ptr;
    delete[] node_to_delete;
}

void EmployeeList::reverse_the_list() {
    if (head_ptr == NULL || head_ptr->next_ptr == NULL) {
        return;
    }

    EmployeeNode *prev_ptr = NULL;
    EmployeeNode *current_ptr = head_ptr;
    EmployeeNode *next_ptr = NULL;

    while (current_ptr != NULL) {
        next_ptr = current_ptr->next_ptr;
        current_ptr->next_ptr = prev_ptr;

        prev_ptr = current_ptr; 
        current_ptr = next_ptr;
    }

    head_ptr = prev_ptr;
}

int EmployeeList::get_id(int index) {
    EmployeeNode* index_ptr = head_ptr;
    for (int i = 0; i < index; i++)
    {
        index_ptr = index_ptr->next_ptr;
    }
    if (index_ptr != NULL)
    {
        return index_ptr->id;
    }
    return -1; // -1 for not found
}

int EmployeeList::get_salary(int index) {
    EmployeeNode* index_ptr = head_ptr;
    for (int i = 0; i < index; i++)
    {
        index_ptr = index_ptr->next_ptr;
    }
    if (index_ptr != NULL)
    {
        return index_ptr->salary;
    }
    return -1; // -1 for not found
}

int EmployeeList::get_department(int index) {
    EmployeeNode* index_ptr = head_ptr;
    for (int i = 0; i < index; i++)
    {
        index_ptr = index_ptr->next_ptr;
    }
    if (index_ptr != NULL)
    {
        return index_ptr->department;
    }
    return -1; // -1 for not found
}

EmployeeNode* EmployeeList::update_the_node(int id, int updated_salary, int updated_department) {
    EmployeeNode* index_ptr = head_ptr;
    while (index_ptr != NULL)
    {
        if (index_ptr->id == id)
        {
            index_ptr->salary = updated_salary;
            index_ptr->department = updated_department;
            return index_ptr;
        }
        index_ptr = index_ptr->next_ptr;
    }
    return NULL;
}

EmployeeNode* EmployeeList::delete_the_node(int id) {

    if (id == employee_count)
    {
        delete_from_front();
        employee_count--;
        return head_ptr;
    }
    
    EmployeeNode *previous = head_ptr;
    EmployeeNode *current = head_ptr->next_ptr;
    EmployeeNode *next = head_ptr->next_ptr->next_ptr;

    while (current != NULL)
    {
        if (current->id == id)
        {
            previous->next_ptr = next;
            delete[] current;
            employee_count--;
            return head_ptr;
        }
        
        previous = current;
        current = next;
        if (next != NULL)
        {
            next = next->next_ptr;
        } else {
            next = NULL;
        }
        
    }
    
    return NULL;
}
