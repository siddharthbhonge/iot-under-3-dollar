void get_youtube(){

  /// This builds out the payload URL - not really needed here, but is very handy when adding different arrays to the payload
  counter=0;//keeps track of the payload size
  payload_size=0;
  for(int i=0; i<(sizeof(URL_youtube)-1); i++){//run through the URL we're going to use - sizeof() returns +1 extra, so subtract it out
    payload[payload_size+i] = URL_youtube[i];//build up the payload
    counter++;//increment the counter
  }//for int
  payload_size = counter+payload_size;//payload size is just the counter value - more on this when we need to build out the payload with more data


  if(connect_ESP()){//this calls 'connect_ESP()' and expects a '1' back if successful
  //nice, we're in and ready to look for data
  //first up, we need to parse the returned data, and this is getting the youtube subscribers, which look like <b>33,101</b>
  //the keyword we need to find first is b>, which is stored in a constant keyword_html_start_b
    if(read_until_ESP(keyword_html_start_b,sizeof(keyword_html_start_b),5000,0)){//go find b> then stop
      if(read_until_ESP(keyword_html_end_b,sizeof(keyword_html_end_b),5000,1)){//our data is next, so change mode to '1' and stop at </b
        //got our data, so quickly store it away in the youtube_subs[] variable
        for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(keyword_html_end_b)+1); i++)//go see 'setup' and how this was done with the ip address for more info
        youtube_subs[i]=scratch_data_from_ESP[i];//store it all in
        youtube_subs[0]=(scratch_data_from_ESP[0]-sizeof(keyword_html_end_b)+1);//set the length at [0]
        //we still have more data to get out of this stream, now we want the views, which looks like <b>4,492,026</b>
        if(read_until_ESP(keyword_html_start_b,sizeof(keyword_html_start_b),5000,0)){//same as before - first b
          if(read_until_ESP(keyword_html_end_b,sizeof(keyword_html_end_b),5000,1)){//now second b and mode=1
            for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(keyword_html_end_b)+1); i++)
            youtube_views[i]=scratch_data_from_ESP[i];//store it in
            youtube_views[0]=(scratch_data_from_ESP[0]-sizeof(keyword_html_end_b)+1);//length     

            //now that we have our data, go wait for the connection to disconnect - the ESP will eventually return 'Unlink'
            if(read_until_ESP(keyword_linkdisc,sizeof(keyword_linkdisc),5000,0)){
              Serial.println("FOUND DATA & DISCONNECTED");
              serial_dump_ESP();//now we can clear out the buffer and read whatever is still there
              //let's see how the data looks
              Serial.print("Subscribers=");
              for(int i=1; i<=youtube_subs[0]; i++)
              Serial.print(youtube_subs[i]);
              Serial.println("");
              Serial.print("Views=");
              for(int i=1; i<=youtube_views[0]; i++)
              Serial.print(youtube_views[i]);
              Serial.println("");   
              //that's it!!
              
            }//link
          }//end b views
        }//startb views        
      }//end b sub
    }//startb sub
  }//connect ESP





}//get youtube

