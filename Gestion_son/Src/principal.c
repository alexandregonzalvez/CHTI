#include "gfssp72.h"

void timer_callback(void);
u32 Periode_PWM_en_Tck = 9;
u32 Periode_en_Tck = 91;

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
	// config TIM3-CH3 en mode PWM
	etat.resolution = PWM_Init_ff( TIM3, 3, Periode_PWM_en_Tck );
	
	etat.son = &Son;
	etat.taille = LongueurSon;
	etat.position = 0;
	
	
	// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
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
	
	
	
	while	(1)
		{
		}
}
