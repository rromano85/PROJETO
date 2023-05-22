1    // BIBLIOTECA   PARA   WIFI
2    # include   < ESP 8266 WiFi.h>
3    // BIBLIOTECA   PARA  O  MQTT
4    # include   < Pub Sub Client.h>
5    // DECLARACAO   DOS   PINOS
6    const   int   lamp  =  0;
7    // DECLARACAO   DE   VARIAVEIS   UTILIZADAS   NO   DECORRER   DO   CODIGO
8    long   now  =  millis ();
9    long   lastMeasure   =  0;
10    int  control_rele ;
11  int control_anterior = 0;
12     typedef   struct   struct_message
13  {
14	float   temperatura ;
15	float   umidade ;
16    }   struct_message ;

17

18     struct_message   incoming Readings;

19

20    // DECLARACAO   WIFI  E  MQTT
21     WiFiClient   esp Client;
22     Pub Sub Client   client( esp Client);

23

24    // DADOS   WIFI
25    const   char   * ssid  =  " nome   do   wifi";
26    const   char   * password   =  " senha   do   wifi";
27    const   char   * mqtt_server   =  " ip  do   broker";

28
 


29    // FUNCAO   PARA   CONFIGURACAO   DO   WIFI   DE   ACORDO   COM   OS   DADOS ACIMA .
30    void   setup_wifi ()
31  {
32	delay (10);
33	WiFi. mode ( WIFI_AP_STA );
34	Serial. println ();
35	Serial. print(" Conectando  na  rede :  ");
36	Serial. println ( ssid );
37	WiFi. begin ( ssid ,   password );

 
38

39

40

41

42

43

44

45

46

47  }
 

while   ( WiFi. status ()  !=   WL_CONNECTED )
{
delay (500);
Serial. print(".");
}
Serial. println ("");
Serial. print(" Conectado   na   rede   -  ESP   IP:  "); Serial. println ( WiFi. localIP ());
 

48

49    // FUNCAO   PARA   IDENTIFICAR   O  TOPICO  E  A   MENSAGEM   RECEBIDA .
50    void   callback ( String   topic ,  byte  * message ,   unsigned   int length )
 
51  {

52

53

54

55
 

Serial. print(" Mensagem   recebida   no   topico :  "); Serial. print( topic );
Serial. print(".   Mensagem :  "); String   message Temp ;
 
56

57	for  ( int  i  =  0;  i  <  length ;  i++)
58	{
 


 
59

60

61	}
 
Serial. print (( char) message [i]); message Temp   +=  ( char) message [i];
 

62	Serial. println ();

 
63

64

65

66

67

68

69

70

71

72

73

74

75

76

77

78  }
 

// ACIONAMENTO   DE   ACORDO   COM  O  TOPICO   ESCOLHIDO
if  ( topic   ==  " topico / lampada ")
{
Serial. print(" Mudando o status do  rele  para :  "); if  ( message Temp   ==  " Ligado ")
{
control_rele =  1;
}
else   if  ( message Temp   ==  " Desligado ")
{
control_rele =  0;
}
}
Serial. println ();
 

79

80    // FUNCAO   PARA   SE   CONECTAR   AO   BROKER   MQTT  ( MOSQUITTO )
81   void  reconnect ()
82  {

83

84	while   (! client. connected ())
85	{
86	Serial. print(" Conectando - se  ao  broker");

87

88	if  ( client. connect(" ESP 01 Client"))   /* O   DISPOSITIVO FOI   NOMEADO   NO   BROKER   COMO   ESP32Client ,  cada dispositivo deve ter um identificador */
 


 
89	{

90

91



92

93



94	}
95	else
96	{

97

98

99
 

Serial. println (" conectado ");
// NESSE   MOMENTO   E   INDICADO   EM   QUAL   TOPICO  O DISPOSITIVO   VAI   SE   INSCREVER .
client. subscribe (" topico / lampada ");
// CASO   EXISTA   FALHA   NA   CONEXAO ,  ELE   TENTARA   SE RECONECTAR EM 5 SEGUNDOS




Serial. print(" falha , rc="); Serial. print( client. state ());
Serial. println ("   Tentando   novamente   em  5 segundos");
 
100

101	delay (5000);
102	}

103	}

104	}	
105		
106	void  setup ()	
107	{	
108	// PINO   DO   RELE   DECLARADO	COMO   SAIDA
109	pin Mode ( lamp ,  OUTPUT );	
110	Serial. begin (115200 );	
111	setup_wifi ();	
112	client. setServer( mqtt_server ,  1883);
113	client. setCallback ( callback );
114	}
115	
116	void  loop ()
117	{
 


118	// FUNCAO   PARA   VERIFICAR   SE  O   DISPOSITIVO   AINDA   ESTA
CONECTADO   DURANTE   O   FUNCIONAMENTO   E   RECONECTAR .
119	if  (! client. connected ())	
120	{	
121	reconnect ();	
122	}	
123	if  (! client. loop ())	
124	client. connect(" ESP 8266 Client");	
125		
126	// CASES PARA O CONTROLE DA LAMPADA CONFORME	MENSAGEM
	RECEBIDA   DO   BROKER
127	if	( control_rele  !=   control_anterior)
128	{	
129		if  ( control_rele ==  1)
130		{
131		digitalWrite ( lamp ,  HIGH );
132		Serial. println (" Acesso ");
133		}
134		if  ( control_rele ==  0)
135		{
136		digitalWrite ( lamp ,  LOW );
137		Serial. println (" Desligado ");
138		}
139		control_anterior  =  control_rele ;
140	}	
141  }		
 


Código-fonte 2 – Monitor de Temperatura e Umidade

1    // BIBLIOTECA   PARA   WIFI
2    # include   < ESP 8266 WiFi.h>
3    // BIBLIOTECA   PARA  O  MQTT
4    # include   < Pub Sub Client.h>
5    // BIBLIOTECA   PARA  O  SENSOR   DE   TEMPERATURA / UMIDADE
6   # include  " DHT .h"

7

8

9    // DEFININDO   O  MODELO   DO   SENSOR
10  # define DHTTYPE DHT11	// DHT 11

11

12    // DEFININDO   PINO   DO   SENSOR
13    const   int   DHTPin  =  5;

14

15    // DECLARACAO   DE   VARIAVEIS   UTILIZADAS   NO   DECORRER   DO   CODIGO
16   float h;
17   float t;
18     typedef   struct   struct_message   {
19	float   temperatura ;
20	float   umidade ;
21    }   struct_message ;
22     struct_message   incoming Readings;
23    long   now  =  millis ();
24    long   lastMeasure   =  0;

25

26    // DECLARACAO   WIFI  E  MQTT
27     WiFiClient   esp Client;
28     Pub Sub Client   client( esp Client);

29

30    // CRIANDO   A   INSTANCIA   DO   SENSOR
31    DHT   dht( DHTPin ,   DHTTYPE );
 


32

33    // DADOS   WIFI
34    const   char*  ssid  =  " WiFiCasa 2 ghz ";
35   const  char* password =  " dextherabc 123 ";
36    const   char*   mqtt_server   =  " 192 .168 .0 .26 ";

37

38    // FUNCAO   PARA   CONFIGURACAO   DO   WIFI   DE   ACORDO   COM   OS   DADOS ACIMA .
39    void   setup_wifi ()  {
40	delay (10);

 
41

42

43

44

45

46

47

48

49

50

51

52

53

54  }
 

WiFi. mode ( WIFI_AP_STA ); Serial. println ();
Serial. print(" Conectando na rede : "); Serial. println ( ssid );
WiFi. begin ( ssid ,   password );
while   ( WiFi. status ()  !=   WL_CONNECTED )  { delay (500);
Serial. print(".");
}
Serial. println ("");
Serial. print(" Conectado   na   rede   -  ESP   IP:  "); Serial. println ( WiFi. localIP ());
 

55

56    // FUNCAO   PARA   IDENTIFICAR   O  TOPICO  E  A   MENSAGEM   RECEBIDA .
57    void   callback ( String   topic ,  byte *  message ,   unsigned   int length ) {
58	Serial. print(" Mensagem   recebida   no   topico :  ");
59	Serial. print( topic );
60	Serial. print(".   Mensagem :  ");
61	String   message Temp ;
 


62

63

64

65

66

67

68  }

69	


for ( int i = 0; i < length ; i++) { Serial. print (( char) message [i]); message Temp   +=  ( char) message [i];
}
70    //	FUNCAO   PARA   SE   CONECTAR   AO   BROKER   MQTT  ( MOSQUITTO )	
71    vo	id  reconnect ()  {	
72	while   (! client. connected ())  {	
73	Serial. print(" Conectando - se  ao  broker");	
74		
75	if  ( client. connect(" ESP 8266 Client")) /* O   DISPOSITIVO	FOI




76

77	NOMEADO   NO   BROKER   COMO   ESP32Client ,  cada
dispositivo deve ter um identificador */ { Serial. println (" conectado ");
78	} else {
79			Serial. print(" falha ,  rc=");		
80			Serial. print( client. state ());		
81			Serial. println ("   Tentando   novamente	em 5	segundos");
82					
83			delay (5000);		
84		}			
85	}				
86  }					
87

88

89    vo

90

91	


id  setup ()  {
// FUNCAO   PARA   CONFIGURAR   O  SENSOR   DE   TEMPERATURA / UMIDADE
dht. begin ();
 


92		
93		Serial. begin (115200 );
94		setup_wifi ();
95		client. setServer( mqtt_server ,  1883);
96		client. setCallback ( callback );
97		
98	}	
99		
100		
101	vo	id  loop ()  {
102		// FUNCAO   PARA   VERIFICAR   SE  O   DISPOSITIVO   AINDA   ESTA
	CONECTADO   DURANTE   O   FUNCIONAMENTO   E   RECONECTAR .
103	if  (! client. connected ())  {	
104	reconnect ();	
105	}	
106	if(! client. loop ())	
107	client. connect(" ESP 8266 Client");	
108		
109	now  =  millis ();	
110	// A   MEDICAO   E  FEITA  A  CADA   10   SEGUNDOS	
111	if  ( now   -  lastMeasure   >  10000)  {	
112	lastMeasure   =  now ;	
113	// FUNCOES   PARA   LER  A   UMIDADE   E   TEMPERATURA	
114	h  =  dht. read Humidity ();	
115	t  =  dht. read Temperature ();	
116		
117	// CASO   OCORRA   ERRO   NA   LEITURA   SERA   APRESENTADO	NO


118	PAINEL
if  ( isnan (h)  ||  isnan (t)) {
119	Serial. println (" Error  ao  ler  dados  do  DHT11 ");
120	return ;
121	}
 


122

123

124



125	


// TRATAMENTO   PARA   ENVIAR   OS   DADOS   DE   TEMPERATURA   E UMIDADE   PARA  O  BROKER
// TRANSFORMA OS DADOS FLOAT EM CHAR
126	static   char   temperature Temp [7];
127	dtostrf(t,  6 ,  2 ,   temperature Temp );
128	static   char   humidity Temp [7];
129	dtostrf(h,  6 ,  2 ,   humidity Temp );
130	
131	// OS   DADOS   SAO   PUBLICADOS   PARA  O  BROKER   NOS   TOPICOS


132

133

134

135

136

137



Código-fonte 3 – Código Tomada IoT


1

2

3

4

5

6

7

8

9

10

11
 


12 # define CURRENT_CAL 35
13     Energy Monitor   emon1 ;

14

15    // DECLARACAO   DOS   PINOS
16   const  int  pino Sensor_tensao  =  32;
17   const  int  pino Sensor_corrente  =  34;
18    const   int   lamp  =  25;

19

20    // DECLARACAO   DE   VARIAVEIS   UTILIZADAS   NO   DECORRER   DO   CODIGO
21    float  resolucao	= 3.3 / 4096;
22  static  char  tensao Temp [7]  =  "0 ";
23  static  char  corrente Temp [7]  =  "0 ";
24     static   char   powerTemp [7]  =  "0 ";
25   float  corrente ;
26  float c;
27    int  control_rele ;
28  int control_anterior = 0;
29   float h;
30   float t;
31    long   now  =  millis ();
32    long   lastMeasure   =  0;
33    long   lastMeasure 2   =  0;
34     typedef   struct   struct_message   {
35	float  tensao ;
36	float  corrente ;
37    }   struct_message ;

38

39     struct_message   incoming Readings;

40

41    // DECLARACAO   WIFI  E  MQTT
42     WiFiClient   esp Client;
43     Pub Sub Client   client( esp Client);
 


44

45    // DADOS   WIFI
46    const   char*  ssid  =  " nome   do   wifi";
47    const   char*   password   =  " senha   do   wifi";
48    const   char*   mqtt_server   =  " ip  do   dispositivo ";

49

50    // FUNCAO   PARA   CONFIGURACAO   DO   WIFI   DE   ACORDO   COM   OS   DADOS ACIMA .
51    void   setup_wifi ()  {
52	delay (10);

 
53

54

55

56

57

58

59

60

61

62

63

64

65

66  }
 

WiFi. mode ( WIFI_AP_STA ); Serial. println ();
Serial. print(" Conectando na rede : "); Serial. println ( ssid );
WiFi. begin ( ssid ,   password );
while   ( WiFi. status ()  !=   WL_CONNECTED )  { delay (500);
Serial. print(".");
}
Serial. println ("");
Serial. print(" Conectado   na   rede   -  ESP   IP:  "); Serial. println ( WiFi. localIP ());
 

67

68    // FUNCAO   PARA   IDENTIFICAR   O  TOPICO  E  A   MENSAGEM   RECEBIDA .
69    void   callback ( String   topic ,  byte *  message ,   unsigned   int length ) {
70	Serial. print(" Mensagem   recebida   no   topico :  ");
71	Serial. print( topic );
72	Serial. print(".   Mensagem :  ");
73	String   message Temp ;
 


74

75

76

77

78

79

80

81

82

83	

for ( int i = 0; i < length ; i++) { Serial. print (( char) message [i]); message Temp   +=  ( char) message [i];
}


Serial. println ();


// ACIONAMENTO   DE   ACORDO   COM  O  TOPICO   ESCOLHIDO
if  ( topic   ==  " topico / tomada ")  {
84	Serial. print(" Mudando  o   status  do	rele	para :	");
85	if  ( message Temp   ==  " Desligado ")  {			
86	control_rele =  0;			
87	}			
88	else   if  ( message Temp   ==  " Ligado ")	{		
89	control_rele =  1;			
90				
91	}			
92	}			
93

94  }	Serial. println ();			

95

96

97    // FUNCAO   PARA   SE   CONECTAR   AO   BROKER   MQTT  ( MOSQUITTO )
98   void  reconnect ()  {

99

100	while   (! client. connected ())  {
101	Serial. print(" Conectando - se  ao  broker");

102

103	if  ( client. connect(" ESP 32 Client"))  {   /* O   DISPOSITIVO FOI   NOMEADO   NO   BROKER   COMO   ESP32Client ,  cada dispositivo deve ter um identificador */
 


104	Serial. println (" conectado ");
105	// NESSE   MOMENTO   E   INDICADO   EM   QUAL   TOPICO  O
	DISPOSITIVO   VAI   SE   INSCREVER .
106	client. subscribe (" topico / tomada ");
107	
108	// CASO   EXISTA   FALHA   NA   CONEXAO ,  ELE   TENTARA   SE
	RECONECTAR EM 5 SEGUNDOS
109			} else {		
110			Serial. print(" falha ,  rc=");		
111			Serial. print( client. state ());		
112			Serial. println ("   Tentando   novamente	em	5  segundos");
113					
114			delay (5000);		
115			}		
116		}			
117	}				
118					
119					
120

121

122

123	void  setup ()  {
// PINO   DO   RELE   DECLARADO   COMO   SAIDA
pin Mode ( lamp ,  OUTPUT );
// E  FEITO  A   PASSAGEM   DOS   PARAMETROS   PARA  O   CALCULO   DA TENSAO E DA CORRENTE
124		emon1 . voltage ( pino Sensor_tensao , VOLT_CAL ,	1.7);
125		emon1 . current( pino Sensor_corrente ,   CURRENT_	CAL );
126			
127		Serial. begin (115200 );	
128		setup_wifi ();	
129		client. setServer( mqtt_server ,  1883);	
130		client. setCallback ( callback );	
131	}		
132			
 


133	
134	void  loop ()  {
135	// FUNCAO   PARA   VERIFICAR   SE  O   DISPOSITIVO   AINDA   ESTA
	CONECTADO   DURANTE   O   FUNCIONAMENTO   E   RECONECTAR .
136	if  (! client. connected ())  {	
137	reconnect ();	
138	}	
139	if  (! client. loop ())	
140	client. connect(" ESP 32 Client");	
141		
142	now  =  millis ();	
143	// A  CADA   0 ,1   SEGUNDOS   SERA   FEITO  A   MEDICAO	
144	if  ( now   -  lastMeasure   >  100)  {	
145	lastMeasure   =  now ;	
146	// FUNCAO PARA CALCULO DA CORRENTE E TENSAO RMS ,	
	PRIMEIRO   PARAMETRO   E  O  NUMERO   DE   SEMICICLOS   E	O
	SEGUNDO   O  TEMPO   DE   MEDICAO	
147	emon1 . calcVI (20 ,   2000);	
148	// VARIAVEL   RECEBE  O  VALOR   DE   TENSAO   RMS   OBTIDO	
149	float   t	=  emon1 . Vrms;	
150	// VARIAVEL   RECEBE  O  VALOR   DE   CORRENTE   RMS   OBTIDO	
151	c  =  emon1 . Irms;	
152		
153	// FUNCAO   PARA   TRATAMENTO   DE   RUIDOS	
154	if  ( control_rele ==  0)  {	
155	c  =  0;	
156	t  =  0;	
157	} else {	
158	c  =  emon1 . Irms;	
159	float   t	=  emon1 . Vrms;	
160	}	
161	corrente = c - 0.6;	
 


162	if  ( corrente <  0)  {
163	c  =  0;
164	}
165	else {
166	c  =  corrente ;
167	}

168

169	// CALCULO   DA   POTENCIA .
170	float p  =  c  * t;

171

172	// TRATAMENTO   DAS   VARIAVEIS   DE   TENSAO ,   CORRENTE   E POTENCIA   PARA   SER   PUBLICADO   NOS   TOPICOS   RESPECTIVOS
173	// A  FUNCAO   TRANSFORMA   AS   VARIAVEIS   QUE   SAO   DO   TIPO FLOAT EM CHAR
174	dtostrf(t,	6 ,	2 ,	tensao Temp );
175	dtostrf(c,	6 ,	2 ,	corrente Temp );
176	dtostrf(p,	6 ,	2 ,	powerTemp );
177				

178	// PRINTA   OS   DADOS   SERIAL   DO   ARDUINO   PARA   ACOMPANHAMENTO
.

179	Serial. print(" tensao  AC:  ");
180	Serial. print(t);
181	Serial. print(" V");
182	Serial. println ();
183	Serial. print(" Corrente  AC: ");
184	Serial. print(c);
185	Serial. print(" A");
186	Serial. println ();
187	Serial. print(" Potencia :  ");
188	Serial. print(p);
189	Serial. println ("  W");
190	
 


191

192

193

194

195

196

197

198

199

200

201

202

203

204

205	}


// CONTROLE   DO   RELE   CONFORME   MENSAGEM   RECEBIDA   DO   BROKER
if ( control_rele  !=  control_anterior)  { if ( control_rele == 1) {
digitalWrite ( lamp ,  HIGH );
}
if  ( control_rele ==  0)  {
client. publish (" topico / corrente ", 0); digitalWrite ( lamp ,  LOW );
}
control_anterior  =  control_rele ;
}

// ENVIA  A  CADA   30   SEGUNDOS   OS   DADOS   PARA   COMPOR   OS GRAFICOS
206		if  ( now   -  lastMeasure 2   >  30000)  {
207		lastMeasure 2   =  now ;
208		Serial. println (" ENVIADO ");
209		// FUNCAO   RESPONSAVEL   POR   PUBLICAR   UM   DADO   EM  UM   TOPICO
210		client. publish (" topico / tensaograph ",  tensao Temp );
211		client. publish (" topico / correntegraph ",  corrente Temp ) ;
212		client. publish (" topico / potencia ",  powerTemp );
213		
214		}
215		// FUNCAO   RESPONSAVEL   POR   PUBLICAR   UM   DADO   EM  UM   TOPICO
216		client. publish (" topico / tensao ",  tensao Temp );
217		client. publish (" topico / corrente ",  corrente Temp );
218		
219	}	



