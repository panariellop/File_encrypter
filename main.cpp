#include <iostream> 
#include <filesystem>
#include <string>
#include <fstream>
#include<sstream>
using namespace std; 

string hashPass(string password); 


void listNotes(){
	/* Lists all .txt files in the current directory */
	for (const auto & entry: std::__fs::filesystem::directory_iterator("./")){
		// only get .txt files 
		std::__fs::filesystem::path p(entry); 
		if(p.extension() == ".txt"){
			cout << p.filename() << endl; 
		}
	}
}

void decrypt(string password){

}

void encrypt(string password){
	//hashes password and saves it to src/pass
	ofstream data_file(".data"); 
	//hash password 
	string hash = hashPass(password); 

	for (const auto & entry: std::__fs::filesystem::directory_iterator("./")){
		std::__fs::filesystem::path file(entry); 
		if(file.extension() == ".txt"){
			//read file 
			string file_string; 
			ifstream cur_file(file.relative_path());
			ostringstream string_stream; 
			string_stream << cur_file.rdbuf(); 
			file_string = string_stream.str(); 
			//extend the password through the length of the file 
			cur_file.close(); 

			//xor the password with the entire file string 
			//extend the password 
			string encr_str; 
			int ptr = 0; 
			for(int i = 0; i<file_string.length(); i++){
				if(ptr == password.length()){ // if reached the end of password 
					ptr = 0; 
				}
				encr_str += password[ptr] ^ file_string[i]; 
				ptr += 1; 
			}

			
			//re-save the encrypted string to the file 
			ofstream re_save(file.relative_path()); 
			re_save << encr_str; 
			re_save.close(); 

		}
	}

	//write to .data 
	data_file << "HASH=" << hash << endl; 
	data_file << "ENCRYPTED=true"; 
	data_file.close(); 


}

bool checkPass(string password){
	//checks to see if the password is correct by comparing it to the hash
	string line; 
	ifstream file("./src/pass"); 
	while(getline(file, line)){
		cout << line; 
	}

	file.close(); 
	return true; 
}

string hashPass(string password){
	/* Returns password hash */
	/* Does not use salting - yikes */
	
	return password; 

}

void mainMenu(){
	cout << "======= Encrypted Notes Application =======" << endl; 
	// cout << "Password: "; 
	// string user_pass; 
	// cin >> user_pass; 
	// cout << user_pass; 
	cout<< "Your notes:" << endl; 
	listNotes(); 
	encrypt("Goodgod"); 

}


int main(){
	mainMenu(); 

	return 0; 
}