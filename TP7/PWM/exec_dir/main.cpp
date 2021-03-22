int main()
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0xff; // PORT D est en mode sortie
TCNT1 = 0;
	

  for(;;)  // boucle sans fin
  {
	  
	 
		//rapport 0%
	ajustementPWM ( 0 );
	_delay_ms(2000);
	
	//rapport 25%
	ajustementPWM ( 64 );
	_delay_ms(2000);
	 
	 //rapport 50%
	ajustementPWM ( 128);
	_delay_ms(2000);
	 
	 //rapport 75%
	ajustementPWM ( 191 );
	_delay_ms(2000);

	//rapport 100%
	ajustementPWM ( 255 );
	_delay_ms(2000);
	


	
 
  }
  return 0; 
}
