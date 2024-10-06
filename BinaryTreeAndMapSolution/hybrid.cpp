/*@Author
StudentName :Kerem Ýhsan Mercan
StudentID :150210042
Date :13.12.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "header.cpp"
#include <vector>
using namespace std;

int main(int argc, char **argv) {

    // clock_t start = clock();

    
    if (argc != 3) {
        cout << argc << " file provided! It is not the right amount." << "\n";
        return 0;
    }
    

    vector<BinaryTree*> bt_vector;

    fstream data_set(argv[1]);

    string one_line;
    string id_temp, salary_temp, department_temp;
    string first_line;
    bool is_first_line_skipped = false;
    int highest_id;
    int line_count = 1;
    bt_vector.push_back(new BinaryTree);

    while (getline(data_set, one_line, '\n')) {
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

        if (line_count % 5000 == 0) {
            bt_vector.push_back(new BinaryTree);
        }

        bt_vector[line_count/5000]->insert(&temp_emp);

        highest_id = max(highest_id, stoi(id_temp));

        line_count++;
    }
    data_set.close();

    fstream operations_file_var(argv[2]);
    string operation_name;

    while (getline(operations_file_var, one_line, '\n')) {

        stringstream one_line_split(one_line);
        getline(one_line_split, operation_name, ';');        

        if (operation_name == "ADD") {
            string new_emp_salary, new_emp_department;
            getline(one_line_split, new_emp_salary, ';');
            getline(one_line_split, new_emp_department, '\n');

            Employee temp_emp(++highest_id, stoi(new_emp_salary), stoi(new_emp_department));

            if (line_count % 5000 == 0) {
            bt_vector.push_back(new BinaryTree);
            }

            bt_vector[line_count/5000]->insert(&temp_emp);

            line_count++;

        } else if (operation_name == "UPDATE") {
            string id_to_update, updated_salary, updated_department;
            getline(one_line_split, id_to_update, ';');
            getline(one_line_split, updated_salary, ';');
            getline(one_line_split, updated_department, '\n');

            Employee* employee_to_update;

            for (int i = 0; i < (int)bt_vector.size(); i++) {
                employee_to_update = bt_vector[i]->search(stoi(id_to_update));
                if (employee_to_update != NULL) {
                    break;
                }
            }
            
            if (employee_to_update == NULL) {
                cout << "ERROR: An invalid ID to update" << "\n";
                continue;
            }
            
            employee_to_update->set_salary(stoi(updated_salary));
            employee_to_update->set_department(stoi(updated_department));

        } else if (operation_name == "DELETE") {
            string id_to_delete;
            getline(one_line_split, id_to_delete, '\n');
            int id_to_delete_int = stoi(id_to_delete);

            if (bt_vector[0]->getHeight() == 0) {
                cout << "ERROR: There is no Employee" << "\n";
                continue;
            }
            
            Employee* employee_to_delete;

            int i;
            for (i = 0; i < (int)bt_vector.size(); i++) {
                employee_to_delete = bt_vector[i]->search(id_to_delete_int);
                if (employee_to_delete != NULL) {
                    break;
                }
            }
            
            if (employee_to_delete == NULL) {
                cout << "ERROR: An invalid ID to delete" << "\n";
                continue;
            }
            bt_vector[i]->remove(id_to_delete_int);

        }
    }
    operations_file_var.close();

    
    ofstream output_file_var("hybrid_solution.csv");
    output_file_var << first_line << "\n";
    Employee* temp_emp;

    for (int i = 1; i <= highest_id; i++) {
        int j;
        for (j = 0; j < (int)bt_vector.size(); j++) {
            temp_emp = bt_vector[j]->search(i);
                if (temp_emp != NULL) {
                    break;
                }
        }
        
        if (temp_emp == NULL) {
            continue;
        }
        
        output_file_var << temp_emp->get_id() << ";" << temp_emp->get_salary() << ";" << temp_emp->get_department() << "\n";

        bt_vector[j]->remove(i);
    }
    
    output_file_var.close();

    // clock_t end = clock();
    // cout << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";

    return 0;
}