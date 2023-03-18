#pragma once
#include <string>
#include <fstream>
using namespace std;

// File handling header
namespace file_handler {
	/// <summary>
	/// A file handling class
	/// </summary>
	class FileHandle
	{
	public:
		/// <param name="path">Path to file</param>
		/// <param name="readWriteMode">0 for write mode, 1 for read mode</param>
		FileHandle(string path, int readWriteMode) {
			if (readWriteMode == 1 && !file_exist(path)) { // Check if we can access the file
				open = false;
				cout << "File does not exist" << endl;
				return;
			}
			try {
				// Check what mode we are in
				if (readWriteMode == 0) {
					this->mode = readWriteMode;
					this->file.open(path, ios_base::out | ios_base::trunc);
				}
				else {
					this->mode = readWriteMode;
					this->file.open(path, ios_base::in);
				}
				if (file.is_open())
					open = true; // Set open var to whether we opened or not
				else {
					open = false;
					cout << "Failed to open file" << endl;
				}
				file.seekp(0, ios::end);
				this->fileLen = file.tellp();
				file.seekp(0);
			}
			catch (exception ex) { // Exception
				open = false;
				cout << "Failed to open file with exception " << ex.what() << endl;
				file.close();
			}
		};

		/// <summary>
		/// Read a single byte from a file
		/// </summary>
		/// <param name="out">Var to output the byte to</param>
		/// <returns>True if success, false if failed</returns>
		bool getbyte(char* out) {
			if (this->mode == 0)
				return false;
			file.read((char*)out, sizeof(char));
			this->CurFilePos = this->file.tellp(); // Allows us to track when we hit the end of the file
			return file.good();
		};

		string getLine_file() {
			if (this->mode == 0)
				return "";
			string ret;
			getline(this->file, ret);
			return ret;
		}

		void setPos(int pos) {
			this->file.seekp(pos);
		}

		bool writeLine(string line) {
			this->file << line << endl;
		}

		/// <summary>
		/// Write a byte to a file
		/// </summary>
		/// <param name="write">Byte to write</param>
		/// <returns>true if success, false if failed</returns>
		bool writebyte(char write) {
			if (this->mode == 1)
				return false;
			this->file.put(write);
			this->file.flush();
			return file.good();
		};

		/// <summary>
		/// Close the file
		/// </summary>
		void close() { 
			this->file.close();
		};

		bool open = false; // Whether the file is open
		int fileLen = 0; // The total lenght of the file
		int CurFilePos = 0; // Current file position

		/// <summary>
		/// Check if a file exists
		/// </summary>
		/// <param name="fileName">File to check for existance</param>
		/// <returns>True if exists</returns>
		static bool file_exist(string fileName)
		{
			std::ifstream infile(fileName);
			bool out = infile.good();
			infile.close();
			return out;
		}

	private:
		fstream file; // File holder
		int mode = -1; // Read write mode tracker
	};

	/// <summary>
	/// Enum for file handle mode
	/// </summary>
	enum FileMode
	{
		write = 0,
		read = 1
	};
}