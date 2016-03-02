
//see get_youtube for more information - this one is just for example - same exact idea
void get_current_temp(){
  counter=0;
  payload_size=0;
  for(int i=0; i<(sizeof(URL_current_temp)-1); i++){
  payload[payload_size+i] = URL_current_temp[i];
  counter++;
  }//for int
     payload_size = counter+payload_size;
     if(connect_ESP()){

      if(read_until_ESP(keyword_html_start_temp,sizeof(keyword_html_start_temp),5000,0)){
      if(read_until_ESP(keyword_html_end_temp,sizeof(keyword_html_end_temp),5000,1)){
      for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(keyword_html_end_temp)+1); i++)// i=0 is total lenght including key, so sub the key length-1, not < since we're not starting at 0
      current_temp[i]=scratch_data_from_ESP[i];
      current_temp[0]=(scratch_data_from_ESP[0]-sizeof(keyword_html_end_temp)+1);   
      
      if(read_until_ESP(keyword_linkdisc,sizeof(keyword_linkdisc),5000,0)){
      Serial.println("FOUND DATA & DISCONNECTED");
      serial_dump_ESP();
      Serial.print("Current Temp=");
      for(int i=1; i<=current_temp[0]; i++)
      Serial.print(current_temp[i]);
      Serial.println("");
           
        }//link
       
     }//end  temp
     }//start temp
  }//connect ESP
  
  
  
  
  
}//get current temp
