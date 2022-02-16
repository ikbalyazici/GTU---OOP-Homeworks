#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct Pixel		//Pixel struct to keep pixels data red, green and blue
{
	Pixel(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}
	int r, g, b;
};

class ppmImage 
{
	public:
		//CONSTRUCTORS
		ppmImage();								//Default constructor
		ppmImage(const string s);				//Takes filename as a parameter 
		ppmImage(const int x , const int y);	//Takes height and width of 2D vector 

		//GETTERS
		int get_w();	//Takes and returns private height value
		int get_h();	//Takes and returns private width value
		
		//CLASS MEMBER FUNCTIONS
		void saveimg(const string s);	//Takes a string it is name of file as parameter and writes everyting in the object to file
		void readimg(const string s);   //Takes a string it is name of file as parameter and copies everything in the file to object
		void print_dimensions();		//It prints dimensions of the 2D vector in the object to the stdout
		Pixel& info (const int x, const int y); //It returns the Pixel stored in the vector[x][y]
		void change (Pixel& pxl, int r, int g, int b); //Changes the value of red, green and blue
		const int& operator()(const int& x , const int& y , const int color_code)const; //overloads () operator
		int& operator()(const int& x , const int& y , const int color_code);			//overloads () operator
		
		//FRIEND FUNCTIONS
		friend ppmImage operator+ (const ppmImage img1 , const ppmImage img2); // overloads + operator
		friend ppmImage operator- (const ppmImage img1 , const ppmImage img2); // overloads - operator
		friend const ostream& operator<< (ostream& os, const ppmImage& img);   // overloads << operator
		 
	private:
		int width;	//Second dimension of the vector and row number of the ppm file (except file informations)
		int height; //First dimension of the vector and column number of the ppm file (except file informations)
		int max; 	//It stores Max value of the one color (r,g,b) can take
		string file_name; //It stores objects file name
		string format; //It stores format of ppm file (P3 generally)
		vector<vector<Pixel> > pixels; //It stores Pixels
};

//TEST FUNCTIONS

//Tests overloaded + operator
int test_addition(const string filename_image1 , const string filename_image2, const string filename_image3);

//Tests overloaded - operator
int test_subtraction(const string filename_image1 , const string filename_image2, const string filename_image3);

//I guess it tests overloaded << operator
int test_print (const string filename_image1);

//It reads the file and stores it in the object
int read_ppm(const string source_ppm_file_name, ppmImage& destination_object);

//It saves object by writing its values to the file
int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object);

//It swaps two of r-g-b values of each pixel depending on the swap_choice
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice);

//It keeps one of r-g-b values of each pixel and resets others to zero depending on the color_choice
ppmImage single_color(const ppmImage& source, int color_choice);

int main(int argc, char const **argv)
{
	if(argc < 3 || argc > 5) //controls required argument number
		exit (1);

	ppmImage image1;
	int select = atoi (argv[1]);	//Keeps the selection
	string ppm_file_name1 = argv[2];
	string ppm_file_name2;
	string ppm_file_name3;

	if(argc > 3){			//2nd and 3rd files not always necessary so sometimes they may not be in the arguments
		ppm_file_name2 = argv[3];	//2nd file is exist
		if(argc == 5)
			ppm_file_name3 = argv[4];	//3rd file is exist
	}
	
	switch (select){	//mode selection according to first argument
		case 1:
			if(argc == 5)
				test_addition (ppm_file_name1 , ppm_file_name2 , ppm_file_name3); // it make: 1st file + 2nd file = 3rd file
			else
				exit(1);
			break;
		case 2:
			if(argc == 5)
				test_subtraction (ppm_file_name1 , ppm_file_name2 , ppm_file_name3); // it make 1st file - 2nd file = 3rd file
			else
				exit(1);
		 	break;
		case 3: 
			read_ppm (ppm_file_name1 , image1);
			swap_channels (image1, 2);  // swaps red and blue channels
			write_ppm( ppm_file_name2 , image1);
			break;
		case 4: 
			read_ppm (ppm_file_name1 , image1);
			swap_channels (image1, 3);	// swaps blue and green channels
			write_ppm( ppm_file_name2 , image1);
			break;
		case 5: 
			read_ppm (ppm_file_name1 , image1);
			write_ppm( ppm_file_name2 , single_color(image1, 1)); // only contains red
			break;
		case 6: 
			read_ppm (ppm_file_name1 , image1);
			write_ppm( ppm_file_name2 , single_color(image1, 2)); // only contains green
			break;
		case 7: 
			read_ppm (ppm_file_name1 , image1);
			write_ppm( ppm_file_name2 , single_color(image1, 3)); // only contains blue
			break;
		default: break;
	}
	return 0;
}

ppmImage::ppmImage(): file_name("") , height(0) , width(0) {} // Default Constructor

ppmImage::ppmImage(const string s): file_name(s) , height(0) , width(0) //Constructor takes filename as parameter
{
	size_t pos = file_name.find(".");
	string test = file_name.substr(pos);
	if(test != ".ppm")	//it checks the file is ppm file or not
		exit(1);
}

ppmImage::ppmImage(const int x , const int y): file_name("") , height(x) , width(y) //Construvtor takes dimensions as parameter
{
	if(height < 0 || width < 0) //dimensions can not be negative, start point is 0,0
		exit(1);

	int i,k;
	for(i=0; i<height; i++){
		for(k=0; k<width; k++){		//creates empty (white) pixels initially
			pixels[i][k].r = 255; 
			pixels[i][k].g = 255;
			pixels[i][k].b = 255;
		}
	}
}

int ppmImage::get_h(){
	return height;	//Accesor for private class member height
}

int ppmImage::get_w(){
	return width;	//Accesor for private class member width
}

void ppmImage::saveimg(const string s){	// From Object --> To File
	
	int i=0, j=0;	//Loop counters

	file_name = s;	
	ofstream myppm (file_name); //Accesing file 
	
	if(myppm.is_open()) //Opening file
	{
		myppm << format <<endl; 
		myppm << height <<" "<<  width << endl;
		myppm << max << endl;

		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)	//Writing pixels to the file
			{
				myppm << pixels[i][j].r << " " << pixels[i][j].g << " " << pixels[i][j].b << "   ";
			}
			myppm << endl;
		}
	}
	else
		exit (1);
}

void ppmImage::readimg(const string s){ //From File --> To Object
	file_name = s;
	Pixel p; 		//Temporary pixel it takes value from file and gives it to the object
	int i=0 , k=0;  //Loop counters

	ifstream myppm (file_name); //Accesing File
	if(myppm.is_open()) // Opening File
	{
		getline (myppm , format); //Getting values to object
		myppm >> height >> width >> max;

		pixels.resize(height*width*sizeof(vector<Pixel>)); //Determining size of vector to push new items in it
		for (i=0 ; i< height ; i++)
		{
			for(k=0 ; k< width ; k++)
			{
				if(myppm)
					myppm >> p.r >> p.g >> p.b; //Getting values to temporary pixel
				
				pixels[i].push_back(p); //Keeping pixels in the object
			}
		}
	}
	else
		exit (1);
}

void ppmImage::print_dimensions(){
	cout << height << "," << width; //Prints dimensions
}

Pixel& ppmImage::info(const int x, const int y){ 
	return pixels[x][y];	//Returns certain pixel
}

void ppmImage::change(Pixel& pxl, int r, int g, int b){ //Gives new values to the Pixel Color values r-g-b
	pxl.r = r; 
	pxl.g = g;
	pxl.b = b;
}

int& ppmImage::operator()(const int& x , const int& y , const int color_code){
	switch(color_code){
		case 1: return pixels[x][y].r; break;
		case 2: return pixels[x][y].g; break;
		case 3: return pixels[x][y].b; break;
		default: exit(1);
	}
}

const int& ppmImage::operator()(const int& x , const int& y , const int color_code)const{
	switch(color_code){
		case 1: return pixels[x][y].r; break;
		case 2: return pixels[x][y].g; break;
		case 3: return pixels[x][y].b; break;
		default: exit(1);
	}
}

int read_ppm(const string source_ppm_file_name, ppmImage& destination_object){ //Reads source file and keep it in the dest. obj...
	destination_object.readimg(source_ppm_file_name); //...By calling class member function of this object
}

int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object){//Takes value from obj anf writes it to file...
	ppmImage temp = source_object; //in order to use values of source object it copies to the temp object
	temp.saveimg(destination_ppm_file_name); //...By calling class member function of copied temo object
}

int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice){//swaps two colours values according to swap choice
	ppmImage p = image_object_to_be_modified; //it helps me by using short names
	int a,b,c; //temp colour values

	if(swap_choice == 1 || swap_choice == 2 || swap_choice == 3) //controls the choice, is it exist
	{
		for(int i = 0 ; i< image_object_to_be_modified.get_h() ; i++) 
		{
			for(int k=0 ; k<image_object_to_be_modified.get_w() ; k++) //Accesing 2D vectors values (Pixels)
			{	
				a = p(i,k,1);
				b = p(i,k,2);
				c = p(i,k,3);
				
				switch(swap_choice){//Giving changed order of colors to change function and it will do it
					
					case 1: image_object_to_be_modified.change(image_object_to_be_modified.info(i,k), b , a , c);
							break;
					
					case 2:	image_object_to_be_modified.change(image_object_to_be_modified.info(i,k), c , b , a);
							break;
					
					case 3: image_object_to_be_modified.change(image_object_to_be_modified.info(i,k), a , c , b);
							break;
				}
			}
		}
		return 1;
	}
	else 
		return 0;
}

ppmImage single_color(const ppmImage& source, int color_choice){//It resets all other colors to 0 except selected color
	ppmImage newobj = source; //Creating new object and now it is copy of the source object

	for(int i = 0 ; i< newobj.get_h() ; i++){
		for(int k=0 ; k<newobj.get_w() ; k++){
			
			switch(color_choice){//Giving changed colors to change function and it will do it
				
				case 1: newobj.change(newobj.info(i,k),newobj(i,k,1),0,0);
						break;
				
				case 2:	newobj.change(newobj.info(i,k),0,newobj(i,k,2),0);
						break;
				
				case 3: newobj.change(newobj.info(i,k),0,0,newobj(i,k,3));
						break;
			}
		}
	}
	return newobj;
}

int test_addition(const string filename_image1 , const string filename_image2, const string filename_image3){
	ppmImage img1, img2;
	read_ppm (filename_image1 , img1); //reads file 1 and stores values in img 1
	read_ppm (filename_image2 , img2); //reads file 2 and stores values in img 2
	write_ppm(filename_image3 , img1 + img2); //Sums img1 and img 2 and keeps the sum in the file 3
}

int test_subtraction(const string filename_image1 , const string filename_image2, const string filename_image3){
	ppmImage img1, img2;
	read_ppm (filename_image1 , img1); //reads file 1 and stores values in img 1
	read_ppm (filename_image2 , img2); //reads file 2 and stores values in img 2
	write_ppm(filename_image3 , img1 - img2); //Subtracts img 2 from img 1 and keeps the result in the file 3
}

int test_print (const string filename_image1){ //It prints object to the stdout
	ppmImage image;
	read_ppm (filename_image1 , image);
	cout << image;
}

ppmImage operator+ (const ppmImage img1 , const ppmImage img2){ //Overloads + operator

	ppmImage returnImg; //it returns the summation result
	Pixel p; //It keeps pixels summations temporarily and after stores in returnImg
	int i=0 , k=0; //Loop counters
	
	if(img1.height == img2.height && img1.width == img2.width) //Controls dimension equality
	{
		//Basic informations about ppm file saving
		returnImg.format = img1.format; 
		returnImg.height = img1.height;
		returnImg.width = img1.width;
		returnImg.max = img1.max;

		returnImg.pixels.resize(img1.height*img1.width*sizeof(vector<Pixel>)); //Determining size of vector to push new items in it
		for(i=0 ; i<img1.height ; i++)
		{
			for(k=0 ; k<img1.width ; k++)
			{
				p.r = img1.pixels[i][k].r + img2.pixels[i][k].r; //summation of reds
				if(p.r > returnImg.max) 
					p.r = returnImg.max; //it can not bigger than max

				p.g = img1.pixels[i][k].g + img2.pixels[i][k].g; //summation of greens
				if(p.g > returnImg.max) 
					p.g = returnImg.max; //it can not bigger than max

				p.b = img1.pixels[i][k].b + img2.pixels[i][k].b; //summation of blues
				if(p.b > returnImg.max) 
					p.b = returnImg.max; //it can not bigger than max

				returnImg.pixels[i].push_back(p); //Storing pixel to object
			}
		}
		return returnImg; //Returning img1 + img 2
	}
	else //Dimensions of img 1 and img 2 are not equal so it will return empty object
	{
		//Basic informations about ppm file saving
		returnImg.format = img1.format; 
		returnImg.height = 0;
		returnImg.width = 0;
		returnImg.max = img1.max;

		return returnImg; //Returning empty object
	}
}

ppmImage operator- (const ppmImage img1 , const ppmImage img2){ //Overloads + operator
	ppmImage returnImg; //It returns the subtraction result
	Pixel p; //It keeps pixels subtractions temporarily and after stores in returnImg
	int i=0, k=0; //Loop counters

	if(img1.height == img2.height && img1.width == img2.width) //Controls dimension equality
	{
		//Basic informations about ppm file saving
		returnImg.format = img1.format;
		returnImg.height = img1.height;
		returnImg.width = img1.width;
		returnImg.max = img1.max;

		returnImg.pixels.resize(img1.height*img1.width*sizeof(vector<Pixel>)); //Determining size of vector to push new items in it
		for(i=0 ; i<img1.height ; i++)
		{
			for(k=0 ; k<img1.width ; k++)
			{
				p.r = img1.pixels[i][k].r - img2.pixels[i][k].r; //subtraction of reds
				if(p.r < 0) 
					p.r = 0;	//it can not smaller than 0

				p.g = img1.pixels[i][k].g - img2.pixels[i][k].g; //subtraction of greens
				if(p.g < 0) 
					p.g = 0; 	//it can not smaller than 0

				p.b = img1.pixels[i][k].b - img2.pixels[i][k].b; //subtraction of blues
				if(p.b < 0) 
					p.b = 0;	//it can not smaller than 0

				returnImg.pixels[i].push_back(p);
			}
		}
		return returnImg; //Returning img1 - img 2
	}
	else
	{
		//Basic informations about ppm file saving
		returnImg.format = img1.format;
		returnImg.height = 0;
		returnImg.width = 0;
		returnImg.max = img1.max;

		return returnImg; //Returning empty object
	}
}

const ostream& operator<< (ostream& os, const ppmImage& img){
	int i,j; //Loop Counters

	//Saving all data to os
	os << img.format <<endl 
	   << img.height <<" "<<  img.width << endl
	   << img.max << endl;

	for(i=0;i<img.height;i++)
	{
		for(j=0;j<img.width;j++)	
		{
			os << img.pixels[i][j].r << " " << img.pixels[i][j].g << " " << img.pixels[i][j].b << "   ";
		}
		os << endl;
	}
	return os; //os returned it will print all data
}

