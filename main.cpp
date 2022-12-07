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

bool encrypt_decrypt(string password){
	//hashes password and saves it to src/pass
	//read from data and get the current encrypted flag 
	bool encrypted = false; 
	ifstream data_file("./.data"); 
	string data_line; 
	/* 
	HASH=...
	ENCRYPTED=true|false
	*/
	while(getline(data_file, data_line)){

		if(data_line.substr(0, data_line.find("=")) == "ENCRYPTED" && data_line.substr(data_line.find("=")+1, data_line.length() - 1) == "1"){
			encrypted = true; 
		}
	}
	data_file.close(); 
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
			if(!encrypted){
				cout << "File " << file.filename() << " encrypted." <<endl; 
			}else{
				cout << "File " << file.filename() << " decrypted." <<endl; 
			}

		}
	}

	//write to .data 
	ofstream new_data_file("./.data"); 
	new_data_file << "HASH=" << hash << endl; 
	//flip the encrypted flag 
	new_data_file << "ENCRYPTED=" << !encrypted; 
	new_data_file.close(); 
	return !encrypted; 


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
	while(true){
		string option; 
		cout << "Options: [l] list notes  [e] encrypt/decrypt notes  [q] quit" <<endl ; 
		cin >> option; 
		if(option == "l"){
			cout<< "Your notes:" << endl; 
			listNotes();
		}; 
		if(option == "e"){
			string pass; 
			cout << "Password: "; 
			cin >> pass; 
			bool encrypted = encrypt_decrypt(pass); 
			cout << "Files were "; 
			if(encrypted){
				cout << "encrypted"; 
			}else{
				cout << "decrypted";
			}
			cout << endl; 

		}
		if(option == "q"){
			break; 
		}
	}

}


int main(){
	mainMenu(); 

	return 0; 
}