package se.t2094572.card.entity;

public class Main {

	public static void main(String[] args) {
		CardDeck cd = new CardDeck();

		System.out.println("createFullDeck");
		cd.createFullDeck();
		System.out.println();
		
		System.out.println("add Joker");
		Card c = new Card(-1, 0);
		cd.addCard(c);
		System.out.println();
		
		System.out.println("shuffle");
		cd.shuffle();
		System.out.println();
		
		System.out.println("showAllCards");
		cd.showAllCards();
		System.out.println();
		
		System.out.println("takeCard");
		System.out.println(cd.takeCard());;
		System.out.println(cd.takeCard());;
		System.out.println(cd.takeCard());;
		System.out.println(cd.takeCard());;
		System.out.println();
		
		System.out.println("size");
		System.out.println(cd.size());
		System.out.println();
		
		System.out.println("seeCard(3)");
		System.out.println(cd.seeCard(3));
		System.out.println();
		
		System.out.println("showAllCards");
		cd.showAllCards();
		System.out.println();
		
		
		
	}

}
