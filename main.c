#include<reg51.h>

//conecta P1 aos pinos de dados do lcd
#define lcd P1

//conecta P3.6 e P3.7 aos pinos do RS e E respectivamente do lcd
// pins P3.6 and P3.7 to rs and e pins of LCD display.
sbit rs=P3^6;
sbit e=P3^7;

//conecta P2.0 e P2.1 aos pinos de dados do sensor de interrupcao 

sbit s1=P2^0;
sbit s2=P2^1;

//comumente o LCD reque valores da tabela ASCII para mostrar os caracteres e numeros
// os numeros de 0 a 9 são de acordo com a tabela ascii do lcd de 48 a 57.
int no[10]={48,49,50,51,52,53,54,55,56,57};

//inicializando as funcoes globais
void delay (int);
void cmd (unsigned char);
void display (unsigned char);
void init (void);
void view (int);
void string (char *);
int count=0;

// a funcao delay produz aprox 1ms de delay
// exemplo usando delay(1000) temos 1 segundo de delay
void delay (int d)
{
	unsigned char i=0;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);
		for(i=248;i>0;i--);
	}
}


//possui um conjunto de comandos iniciais
void cmd (unsigned char c)
{
	lcd=c;
	rs=0;
	e=1;
	delay(5);
	e=0;
}

//a função display é usada para fornecer dados ao lcd, que deve ser exibido.
void display (unsigned char c)
{
	lcd=c;
	rs=1;
	e=1;
	delay(5);
	e=0;
}


// a função string é usada para exibir os caracteres string (palavras).
// esta função funcionará continuando a chamar a função display para exibir o caractere individual em uma palavra,
// até detectar '/0'. geralmente um final de string (palavra).
void string (char *p)
{
	while(*p)
	{
		display(*p++);
	}
}

//a função conterá um conjunto de comandos primários para exibição que é usado para inicializá-la.
void init (void)
{
	cmd(0x38);
	cmd(0x0c);
	cmd(0x01);
	cmd(0x80);
}

// obs: não é possivel enviar os valores de 2 ou mais dígitos diretamente para o display LCD. como, 22, 24, 43, etc...
// para superar este problema esta função  irá dividir o valor de 2 dígitos
//separadamente e enviar eles um a um para a função display
void view (int n)
{
	cmd(0xc0);
	display(no[(n/10)%10]);
	display(no[n%10]);
}


void main()
{
	P2=0x00;// seta o sensor IR conectado à PORT2 como 0 (LOW).

	init();// chama a funcao para inicializar o lcd

	string("   Projeto de  ");
	cmd(0xc0);
	string("      Micro    ");
	delay(1000);
	cmd(0x01);
	cmd(0x80);
	string("Num de objts...");
	cmd(0xc0);
	view(count);// exibe 0 inicialmento no LCD


	// se s1 for alto e dps s2 alto, o valor sera incrementado
	// se s2 for alto e dps s1 alto, o valor sera decrementado
	while(1)
	{
		if(s1==1)
		{
			while(s2==0);//segura ate o s2 ser 1
			if(count!=99)//verifica se o contador não é 99 (numero maximo)
			count=count+1;//incrementa
			while(s2==1);//segura ate o s2 ser 0
			view(count);//exibe o contador no LCD
		}
		
		if(s2==1)
		{
			while(s1==0);//segura ate o s1 ser 1
			if(count!=0)//verifica se o contador não é 00 (numero minimo)
			count=count-1;//decrementa
			while(s1==1);//segura ate o s1 ser 0
			view(count);//exibe o contador no LCD
		}
	}
}