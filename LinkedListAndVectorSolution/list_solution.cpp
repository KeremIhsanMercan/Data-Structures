/* @Author
Student Name : Kerem Ýhsan Mercan
Student ID : 150210042
Date : 4.11.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <list>
using namespace std;

class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        void set_id(int new_id) {
            id = new_id;
        }
        void set_salary(int new_salary) {
            salary = new_salary;
        }
        void set_department(int new_department) {
            department = new_department;
        }
        void construct_employee(int x, int y, int z) {
            set_id(x);
            set_salary(y);
            set_department(z);
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

list<Employee>::iterator is_valid(int id_to_check, list<Employee> &employee_list);

int main(int argc, char **argv) {

    // clock_t start = clock();

    if (argc != 3) {
        cout << argc << " file provided! It is not the right amount." << endl;
        return 0;
    }

    list<Employee> employee_list;

    fstream hw2(argv[1]);

    string one_line;
    string id_temp, salary_temp, department_temp;
    string first_line;
    int highest_id;
    
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

        Employee temp_emp;
        temp_emp.construct_employee(stoi(id_temp), stoi(salary_temp), stoi(department_temp));

        employee_list.push_back(temp_emp);
        highest_id = stoi(id_temp); // last emp will have the highest id
    }

    hw2.close();

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

            Employee temp_emp;
            temp_emp.construct_employee(++highest_id, stoi(new_emp_salary), stoi(new_emp_department));

            employee_list.push_back(temp_emp);

        } else if (operation_name == "UPDATE")
        {
            string id_to_update, updated_salary, updated_department;
            getline(one_line_split, id_to_update, ';');
            getline(one_line_split, updated_salary, ';');
            getline(one_line_split, updated_department, '\n');

            int id_to_update_int = stoi(id_to_update);
            int emp_new_salary_int = stoi(updated_salary);
            int emp_new_department_int = stoi(updated_department);

            auto list_index = is_valid(id_to_update_int, employee_list);
            
            if (list_index != employee_list.end()) {

                Employee temp_emp;
                temp_emp.construct_employee(id_to_update_int, emp_new_salary_int, emp_new_department_int);

                (*list_index) = temp_emp;
        
            } else {
                cout << "ERROR: An invalid ID to update";
                continue;
            }

        } else if (operation_name == "DELETE")
        {
            string id_to_delete;
            getline(one_line_split, id_to_delete, '\n');

            if (employee_list.size() <= 0)
            {
                cout << "ERROR: There is no Employee";
                continue;
            }

            int id_to_delete_int = stoi(id_to_delete);
            auto list_index = is_valid(id_to_delete_int, employee_list);

            if (list_index != employee_list.end()) {

                employee_list.erase(list_index);

            } else {
                cout << "ERROR: An invalid ID to delete";
                continue;
            }
        }
    }

    operations_file_var.close();

    ofstream solution_file("list_solution.csv");
    auto itr = employee_list.begin();

    for (int i = 0; i < (int)employee_list.size(); i++)
    {
        if (i == 0)
        {
            solution_file << first_line;
        }

        solution_file << (*itr).get_id() << ";" << (*itr).get_salary() << ";" << (*itr).get_department() << endl;
        advance(itr, 1);
    }
    
    solution_file.close();

    // clock_t end = clock();
    // cout << "execution time of all list solution program: " << (double)(end-start)*1000 / CLOCKS_PER_SEC << " miliseconds!" << endl;
    
    return 0;
}

list<Employee>::iterator is_valid(int id_to_check, list<Employee> &employee_list) { // returns the order of the element

    auto itr = employee_list.begin();

    while (itr != employee_list.end())
    {
        if ((*itr).get_id() == id_to_check)
        {
            return itr;
        }
        advance(itr,1);
    }
    
    return itr; // returns -1 if non valid
}
