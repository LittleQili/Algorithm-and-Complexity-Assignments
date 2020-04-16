/*********************************************
 * Description of the model:
 * 1. Variables: mainly indexed by product type,
 				 month and machine type.
   2. Restrictions:  there're three part
   		Machine maintenance part
   		Make&Store&Sell part
   		profit part
   3. objective value: 
   		maximize net profit.
   		Other just define and use them.
   For detail, please refer to the comment below.
 *********************************************/

 {string} Products = ...;
 {string} Month = ...;
 {string} MachineT = ...;//type of machine

 tuple productData { 
   float profit;
   float time_machine[MachineT];//produce time for each type of machine
   float marketlim[Month];//sell limitation
}

int Num_machine[MachineT] = ...;//number of machine
productData Product[Products] = ...;//detail info. of each product

dvar int+ Make_prod[Month][Products];//make amount
dvar int+ Sell_prod[Month][Products];//sell amount
dvar int+ Hold_prod[Month][Products];//hold amount at end of each month
dvar int+ Maintaince[Month][MachineT];//the number of machine Available each month
dvar float+ sellp;// sell profit result
dvar float+ holdc;// hold cost result
dvar float+ netp;// net profit result

//max
maximize
	netp;

//s.t.
subject to {
	//Machine maintenance part
	forall (ma in MachineT){
		cond_machine_use://basic restriction
        forall (mo in Month){
             Maintaince[mo][ma] <= Num_machine[ma]; 
             Maintaince[mo][ma] >= 0; 
        }	
		relax_machine://maintenance restriction
	  	sum( mo in Month ) 
        	Maintaince[mo][ma] == 5 * Num_machine[ma];    
	}
	
	//Make&Store&Sell part
	make_sell_store://make = sell+ delta_store
	forall(pd in Products){
		Hold_prod["Jan"][pd] ==	Make_prod["Jan"][pd] - Sell_prod["Jan"][pd];
		Hold_prod["Feb"][pd] ==	Make_prod["Feb"][pd] - Sell_prod["Feb"][pd] + Hold_prod["Jan"][pd];
		Hold_prod["Mar"][pd] ==	Make_prod["Mar"][pd] - Sell_prod["Mar"][pd] + Hold_prod["Feb"][pd];
		Hold_prod["Apr"][pd] ==	Make_prod["Apr"][pd] - Sell_prod["Apr"][pd] + Hold_prod["Mar"][pd];
		Hold_prod["May"][pd] ==	Make_prod["May"][pd] - Sell_prod["May"][pd] + Hold_prod["Apr"][pd];
		Hold_prod["Jun"][pd] ==	Make_prod["Jun"][pd] - Sell_prod["Jun"][pd] + Hold_prod["May"][pd];
	}
	store_final://in Jun.,store 50 of each type
	forall(pd in Products){
		Hold_prod["Jun"][pd] == 50;
	}
	maximum_store://Store restriction
	forall(pd in Products){
		forall(mo in Month){
			Hold_prod[mo][pd] <= 100;		
		}	
	}
	make_time://the time of producing \leq total working time of working machine
	forall(mo in Month){
		forall(ma in MachineT){
			sum(pd in Products)
				Product[pd].time_machine[ma] * Make_prod[mo][pd] <= 24 * 16	* Maintaince[mo][ma];		
		}
	}
	sell_amount://sell amount restriction
	forall (mo in Month){
		forall(pd in Products){
			Sell_prod[mo][pd] <= Product[pd].marketlim[mo];		
		}	
	}
	
	//profit part
	sell_profit://definition of sell profit
	sellp == sum(mo in Month)
	  sum(pd in Products)
	    (Product[pd].profit*Sell_prod[mo][pd]);
	hold_cost://definition of hold cost
	holdc == sum(mo in Month)
	  sum(pd in Products)
	    (Hold_prod[mo][pd] * 0.5);
	net_profit://definition of net profit
	netp == sellp - holdc;
}