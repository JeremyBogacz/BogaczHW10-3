// Created by Jeremy Bogacz on 11/30/2021
//
// This program creates a web browser that takes
// in three LED states. On, Off, and Blink. It also 
// has a text box for the number of blinks. The LED 
// is on GPIO60 and uses the fet circuit shown in
// class.

#include <iostream>        
#include <stdlib.h>        
#include <sys/sysinfo.h>   
#include <cgicc/Cgicc.h>   
#include <cgicc/CgiDefs.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include<unistd.h>

#define LED_GPIO "/sys/class/gpio/gpio60/"

using namespace std;
using namespace cgicc;

void writeGPIO(string filename, string value){
   fstream fs;
   string path(LED_GPIO);
   fs.open((path + filename).c_str(), fstream::out);
   fs << value;
   fs.close();
}

int main(){
   Cgicc form;		// the CGI form object
   string cmd;		// the Set LED command
   string numBlinks;	// the number of blinks command		

   writeGPIO("direction", "out");	// Set GPIO direction

   // get the state of the form that was submitted - script calls itself
   bool isStatus = form.queryCheckbox("status");
   
   form_iterator it1 = form.getElement("cmd");  // the radio command
   form_iterator it2 = form.getElement("numBlinks"); // the text command
   
   // Process cmd with off as the default
   if (it1 == form.getElements().end() || it1->getValue()==""){
	   cmd = "off";                   
   }
   else{
	   cmd = it1->getValue();
   }      
   
   // Process numBlinks command with 0 as the default
   if (it2 == form.getElements().end() || it2->getValue()==""){
	   numBlinks = "0";
   }
   else{
	   numBlinks = it2->getValue();
   }
   
   // Generate the html web browser with the form
   cout << HTTPHTMLHeader() << endl;       
   cout << html() << head() << title("Blink LED") << head() << endl;
   cout << body() << h1("CPE 422 Project: Post LED Example") << endl;;
   cout << h2("Controlling a GPIO via a web browswer") << endl;
   cout << "<form action=\"/cgi-bin/myLed.cgi\" method=\"POST\">\n";
   cout << "<div>Set LED: <input type=\"radio\" name=\"cmd\" value=\"on\""
        << ( cmd=="on" ? "checked":"") << "/> On ";
   cout << "<input type=\"radio\" name=\"cmd\" value=\"off\""
        << ( cmd=="off" ? "checked":"") << "/> Off ";
   cout << "<input type=\"radio\" name=\"cmd\" value=\"blink\""
	<< ( cmd=="blink" ? "checked":"") << "/> Blink ";
   cout << "<input type=\"submit\" value=\"Set LED\" /></div>";
   cout << "<br>";
   cout << "<div>Number of Blinks: <input type=\"text\" name=\"numBlinks\"/>";
   cout << "</div></form>";
   
   // Process the form data to change the LED state
   if (cmd=="on"){
	   writeGPIO("value", "1");
   }	   
   else if (cmd=="off"){
	   writeGPIO("value", "0");
   }
   else if (cmd=="blink"){
	   
	   int num = stoi(numBlinks);
	   
	   writeGPIO("value", "0");
	   for(int i = 0; i < num; i++){
		   writeGPIO("value", "1");
		   sleep(1);
		   writeGPIO("value", "0");
		   sleep(1);
	   }
   }
   else{
	   cout << "<div> Invalid command! </div>";
   }  

   cout << body() << html();
   
   return 0;
}
