package myApp;

public class AccountingApp {

	public static void main(String[] args) {
		
		double valueOfSupply = Double.parseDouble(args[0]);
		double vatRate = 0.1;
		double expenseRate = 0.3;
		double VAT = valueOfSupply*vatRate;
		double total = valueOfSupply+VAT;
		double expense = valueOfSupply*expenseRate;
		double income = valueOfSupply - valueOfSupply*expenseRate;
		double dividend1 = (valueOfSupply - valueOfSupply*0.3)*0.5;
		double dividend2 = (valueOfSupply - valueOfSupply*0.3)*0.3;
		double dividend3 = (valueOfSupply - valueOfSupply*0.3)*0.2;


		System.out.println("Value of supply : " +valueOfSupply);
		System.out.println("VAT : "+ VAT);
		System.out.println("Total : "+ total);
		System.out.println("Expense : "+expense );
		System.out.println("Income : "+income);
		System.out.println("Dividend1 : "+dividend1);
		System.out.println("Dividend2 : "+dividend2);
		System.out.println("Dividend3 : "+dividend3);

	}

}
