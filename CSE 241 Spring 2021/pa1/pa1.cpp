#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>

using namespace std;

//Function prototypes
int r(const int arg2); //runs operations for -r argument
int u(string arg2); //runs operations for -u argument
int create_random_number(const int arg2); //creates random number to guess that length is equal to arg2 
int guess_and_control (string hidden_num); //user guesses the hidden number and guess controlled is same with hidden number
bool error_checker (string hidden_num , string guess); //it controls possible errors and if there is error it prints error number
bool isUnique (string suspect); //it controls is the guess or -u argument unique number or not

int main(int argc, char const *argv[])
{
	if(argc < 3 || argc > 3) //it controls is there any less or more argument
		cout << "E0" << endl; //error occured its the error message 
	else //argument number is not more or less 
	{
		if(argv[2][0]>'0' && argv[2][0]<='9') //argv 2 is argument after -r/-u. It checks it first for argument standarts
		{
			const int arg2 = stoi(argv[2]); 
			//arg2 is the argument after -r/-u. It acts length for -r but it is the secret number for -u. stoi() makes it integer
			srand(static_cast<int>(time(NULL))); //its for random number creation
			
			if(strcmp(argv[1],"-r")==0) // It calls r function because user starts program with -r argument
				r(arg2);
			
			else if (strcmp(argv[1],"-u")==0) //It calls u function after some controls because user starts program with -u argument
			{
				string control = to_string(arg2); 
				//if argument includes non integer digits, stoi function not take them and when it converted to string again
				//converted form and original argument will be different, but if they are not different it is in true form
				
				if(argv[2]==control) // it is in true form
					u(argv[2]);		 
				
				else 				 // there is difference its an error
					cout << "E0" << endl; //error message
			}
			else //user enters invalid argument so it is error
				cout << "E0" << endl; //error message
		}
		else //argv 2 is not in true form its an error
			cout << "E0" << endl; // error message
	}
	return 0;
}

int r(const int arg2){ //-r argument gameplay
	int number = create_random_number(arg2); //its the complete hidden number in integer form
	string hidden_num = to_string(number); //here the hidden number converted to string form because digit controlling is easier
	guess_and_control (hidden_num); //user makes guesses and program controls these	
}

int u(string arg2){ //-u argument gameplay
	if(isUnique(arg2))  //first checking the argument number is unique or not
		guess_and_control (arg2); //its unique so user can start guessing anf program will controll guesses
	
	else	//argument number is not unique its an error
		cout << "E0" << endl; //error message
}

bool isUnique(string suspect) //controls the -u argument and guesses are unique or not unique
{
	int i,j; //loop counters
	bool unique = true; //unique or not controller
	for(i=0;i<suspect.length();i++) //each digit will compare with each other its the outer loop determines which digit controls
	{
		for(j=0;j<suspect.length();j++) //its the inner loop controls each digit with determined digit by outer loop
		{
			if(i==j) //no need to control the digit wtih itself
				continue; //so program skips it
			if(suspect[i]==suspect[j])  //two different digits are same so this number is not unique
			{
				unique = false; //controller upgraded to false becase the number is not unique
				break; //no need to control more
			}
		}
		if(!unique)	break; // no need to control for next digits program determined the number is not unique
	}	
	return unique; //feedback
}

int create_random_number(const int arg2)
{
	int i,j, digit[arg2],number=0; 
	//i and j are loop counters, digit[arg2] is for keeping and controlling digits of hidden number, number is complete hidden number
	bool isDiff = true;
	//it controls each digit is unique
	for(i=1 ; i<=arg2 ; i++) //digit counter starts from high-most digit
	{
		if(i==1) //high-most digit can not be zero so this digit creates specially
		{
			digit[i-1] = rand()%9+1; //rand gives result between 0-8 and +1 makes it between 1-9 as we want
			number += digit[i-1] * pow(10,arg2-i); //high-most digit kept in the its location in complete hidden number
		}
		else //other digits except high most digit
		{
			isDiff = true; //to make control first reset this controller to true
			digit[i-1] = rand()%10; //rand function gives a digit between 0-9
			for(j=0 ; j<i-1 ; j++) // difference control loop, it controls the digits before this digit
			{
				if(digit[i-1]==digit[j]) //if this digit same with any other higher digit...
				{
					isDiff = false; //...controller changes...
					break; //...controlling finishes...
				}
			}
			if(isDiff==false)//because of the 2 digit same
				i--; //newest digit will create again (until it became different than other digits)
			else //digit is different from others
				number += digit[i-1] * pow(10,arg2-i); //so it kept in the its location in complete hidden number
		}
	}
	return number; //complete hidden number sends to the upper function (r function)
}

int guess_and_control (string hidden_num) //in this function user makes his/her guess and program controls it
{
	int i=0,j=0; //i and j are loop counters 
	int c_ex=0,c_mis=0,try_c=0; //c_ex counts right digit in right place, c_mis counts right digit in wrong place, try_c counts guess number
	const int MAX_GUESS = 100; //its the max value of user guess the hidden number
	bool win = false; //it checks and determines user found the hidden number or not
	bool err; //it keeps error controlling feedback
	string guess; //its user entry to find hidden number

	while(try_c <MAX_GUESS) //user can make max 100 guesses
	{	
		try_c ++;	c_ex=0;		c_mis=0; //resetting counters and counting the next guess

		cin >> guess; //for user enters his/her guess

		err = error_checker(hidden_num,guess); //error checking for user guess
		if(!err) //user made a guess without errors
		{
			if(guess==hidden_num) //user makes the right guess and found the hidden number
			{
				cout << "FOUND " << try_c << endl; //feedback to user (s)he find the hidden number and number of guess until user find
				win = true; //controller upgraded to true because user found hidden number
				break; //guessing loop will end 
			}
			else //users guess are not true
			{
				for(i=0 ; i<guess.length() ; i++) //determining the true guessed digits 
				{
					for(j=0 ; j<hidden_num.length() ; j++)
					{
						if(guess[i]==hidden_num[j]) //hidden number includes one of the digits of guess
						{
							if(i==j)	c_ex++;  // true guessed digit in the right place 
							else		c_mis++; // true guessed digit in the wrong place
							break; //inner loop finishes to looking dor the next digit determines from outer loop
						}
					}
				}
				cout << c_ex << " " << c_mis << endl; //hints are printed
			}
		}
		else //user made a error guess program will end because of error
			break;
	}
	if(!win && !err) //user cannot find the hidden number in 100 guess (s)he failed
		cout << "FAILED" << endl; //feedback to user
}
bool error_checker (string hidden_num , string guess) //it checks the errors in users guess
{
	bool error = false; //error controller
	int i,j; //loop counters
	
	for(i=0 ; i<guess.length() ; i++) //it controls is there any non integer entry digit by digit
	{
		if(guess[i]<'0' || guess[i]>'9') //non integer digit founded
		{
			cout << "E2" << endl; //error feedback message
			error = true; //controller upgrading
			break; //no need to look much more to find an error because we already found it
		}
	}

	if(!error) //first error control is succesful but there are much controls
	{
		if(guess.length() == hidden_num.length()) //controlling the length of guess
		{
			error = !(isUnique(guess)); //digit number is fit so its the time for is it unique
			if(error) //its not unique
				cout << "E1" << endl; //error feedback message
		}

		else //guess has more and less digits from the hidden number and it is an error
		{
			error = true; //controller upgrading
			cout << "E1" << endl; //error feedback message
		}
	}
	return error; //feedbacking 
}
