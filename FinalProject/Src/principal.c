#include "gfssp72.h"

int m2k(int k, u16 * tabSig);

extern u16 TabSig;

unsigned short int dma_buf[64];

unsigned int SYSTICK_PER = 360000;

void sys_callback();

// occurences de tir
int occurences[6];

// score des joueurs
int scores[6];

// seuil de détection du tir
int M2TIR = 1000000;


// gestion du son
void timer_callback(void);
u32 Periode_PWM_en_Tck = 648;
u32 Periode_en_Tck = 6552;

typedef struct {
	int position;		// index courant dans le tableau d'echantillons
	int taille;		// nombre d'echantillons de l'enregistrement
	void * son;	// adresse de base du tableau d'echantillons en ROM
	int resolution;		// pleine echelle du modulateur PWM
	int Tech_en_Tck;	// periode d'ech. audio en periodes d'horloge CPU
} type_etat;

type_etat etat;

extern short Son;
extern short LongueurSon;
extern short PeriodeSonMicroSec;

int main(void)
{
    //m2k(24, &TabSig);
      
		// activation de la PLL qui multiplie la fréquence du quartz par 9
		CLOCK_Configure();
		// PA2 (ADC voie 2) = entrée analog
		GPIO_Configure(GPIOA, 2, INPUT, ANALOG);
		// PB1 = sortie pour profilage à l'oscillo
		GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
		// PB14 = sortie pour LED
		GPIO_Configure(GPIOB, 14, OUTPUT, OUTPUT_PPULL);
		// config port PB0 pour être utilisé par TIM3-CH3
		GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);

		// initialisation du timer 4
		// Periode_en_Tck doit fournir la durée entre interruptions,
		// exprimée en périodes Tck de l'horloge principale du STM32 (72 MHz)
		Timer_1234_Init_ff( TIM4, Periode_en_Tck );
		// enregistrement de la fonction de traitement de l'interruption timer
		// ici le 2 est la priorité, timer_callback est l'adresse de cette fonction, a créér en asm,
		// cette fonction doit être conforme à l'AAPCS
		Active_IT_Debordement_Timer( TIM4, 2, timer_callback );
		// lancement du timer
		Run_Timer( TIM4 );
	
		// activation ADC, sampling time 1us
		Init_TimingADC_ActiveADC_ff( ADC1, 0x43 ); //51
		Single_Channel_ADC( ADC1, 2 );
		// Déclenchement ADC par timer2, periode (72MHz/320kHz)ticks
		Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
		// Config DMA pour utilisation du buffer dma_buf (a créér)
		Init_ADC1_DMA1( 0, dma_buf );

		// Config Timer, période exprimée en périodes horloge CPU (72 MHz)
		Systick_Period_ff( SYSTICK_PER );
		// enregistrement de la fonction de traitement de l'interruption timer
		// ici le 3 est la priorité, sys_callback est l'adresse de cette fonction, a créér en C
		Systick_Prio_IT( 3, sys_callback );
		SysTick_On;
		SysTick_Enable_IT;
		
		// gestion du son
		// config TIM3-CH3 en mode PWM
		etat.resolution = PWM_Init_ff( TIM3, 3, Periode_PWM_en_Tck );
	
		etat.son = &Son;
		etat.taille = LongueurSon;
		etat.position = 0;
  
    while(1)
        {
        }
}

void sys_callback(){
	// Démarrage DMA pour 64 points
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	
	int m2ksig1_1 = m2k(17 , dma_buf);
	int m2ksig1_2 = m2k(18 , dma_buf);
	int m2ksig1_3 = m2k(19 , dma_buf);
	int m2ksig1_4 = m2k(20 , dma_buf);
	int m2ksig1_5 = m2k(23 , dma_buf);
	int m2ksig2 = m2k(24 , dma_buf);
	
	if (m2ksig1_1 > M2TIR){
		occurences[0]++;
	}
	else {
		occurences[0] = 0;
	}
	if (m2ksig1_2 > M2TIR){
		occurences[1]++;
	}	
	else {
		occurences[1] = 0;
	}
	if (m2ksig1_3 > M2TIR){
		occurences[2]++;
	}	
	else {
		occurences[2] = 0;
	}
	if (m2ksig1_4 > M2TIR){
		occurences[3]++;
	}	
	else {
		occurences[3] = 0;
	}
	if (m2ksig1_5 > M2TIR){
		occurences[4]++;
	}	
	else {
		occurences[4] = 0;
	}
	if (m2ksig2 > M2TIR){
		occurences[5]++;
	}
	else {
		occurences[5] = 0;
	}
	
	for (int i = 0; i < 6; i++){
		if (occurences[i] > 12){
			occurences[i] = 0;
			etat.position = 0;
			scores[i] ++;
		}
			
	}
	
}
