package se.t2094572.card.entity;

import java.util.ArrayList;

import se.t2094572.card.game.KeyBoard;

public class User extends Player {
	public User(String name) {
		super(name);
	}

	@Override
	public ArrayList<Integer> selectCards(int size) {
		int c1, c2;
		ArrayList<Integer> selection = new ArrayList<Integer>();
		System.out.printf("%sさん、何番目のカードを選びますか? 2枚選んでください。\n", name);
		
		do {
			System.out.print("1枚目:");
			 c1 = KeyBoard.inputNumber();
		} while (c1 < 1 || c1 > size);
		
		do {
			System.out.print("2枚目:");
			 c2 = KeyBoard.inputNumber();
		} while ((c2 < 1 || c2 > size) && c2 != c1);
		
		selection.add(c1);
		selection.add(c2);
		return selection;
	}
}
