

//This function goes and finds a keyword received from the ESP
// const char keyword1[] = the keyword that this function will look for and return '1' when found
// int key_size = size of that keyword - sizeof()
// int timeout_val - timeout if the keword is not found (in milliseconds 5000=5sec)
// byte mode - if '1', this function will save every byte received to 'scratch_data_from_ESP[]' with length of 'scratch_length' - useful for parsing between keywords
boolean read_until_ESP(const char keyword1[], int key_size, int timeout_val, byte mode){
  timeout_start_val=millis();//for the timeout
  char data_in[20];//this is the buffer - if keyword is longer than 20, then increase this
  int scratch_length=1;//the length of the scratch data array
  key_size--;//since we're going to get an extra charachter from the sizeof()
 
 //FILL UP THE BUFFER
 for(byte i=0; i<key_size; i++){//we only need a buffer as long as the keyword
  
            //timing control
            while(!ESP8266.available()){//wait until a new byte is sent down from the ESP - good way to keep in lock-step with the serial port
              if((millis()-timeout_start_val)>timeout_val){//if nothing happens within the timeout period, get out of here
                Serial.println("timeout");
                return 0;//this will end the function
              }//timeout
            }// while !avail
   
    data_in[i]=ESP8266.read();// save the byte to the buffer 'data_in[]

    if(mode==1){//this will save all of the data to the scratch_data_from
      scratch_data_from_ESP[scratch_length]=data_in[i];//starts at 1
      scratch_data_from_ESP[0]=scratch_length;// [0] is used to hold the length of the array
      scratch_length++;//increment the length
    }//mode 1
    
  }//for i
  
//THE BUFFER IS FULL, SO START ROLLING NEW DATA IN AND OLD DATA OUT
  while(1){//stay in here until the keyword found or a timeout occurs

     //run through the entire buffer and look for the keyword
     //this check is here, just in case the first thing out of the ESP was the keyword, meaning the buffer was actually filled with the keyword
     for(byte i=0; i<key_size; i++){
       if(keyword1[i]!=data_in[i])//if it doesn't match, break out of the search now
       break;//get outta here
       if(i==(key_size-1)){//we got all the way through the keyword without breaking, must be a match!
       return 1; //return a 1 and get outta here!
       }//if
     }//for byte i
     

    //start rolling the buffer
    for(byte i=0; i<(key_size-1); i++){// keysize-1 because everthing is shifted over - see next line
      data_in[i]=data_in[i+1];// so the data at 0 becomes the data at 1, and so on.... the last value is where we'll put the new data
    }//for
 
         
           //timing control
            while(!ESP8266.available()){// same thing as done in the buffer
              if((millis()-timeout_start_val)>timeout_val){
                Serial.println("timeout");
                return 0;
              }//timeout
            }// while !avail
    
   
   
    data_in[key_size-1]=ESP8266.read();//save the new data in the last position in the buffer
   
      if(mode==1){//continue to save everything if thsi is set
      scratch_data_from_ESP[scratch_length]=data_in[key_size-1];
      scratch_data_from_ESP[0]=scratch_length;
      scratch_length++;
    }//mode 1
    
    /*  JUST FOR DEBUGGING
    if(ESP8266.overflow())
    Serial.println("*OVER");
    */
    
  }//while 1

  
  
  
}//read until ESP
