package se.t2094572.card.entity;

import java.util.ArrayList;
import java.util.Random;

public class CPU extends Player{
	private Random r = new Random();

	/**
	 * 
	 */
	public CPU(String name) {
		super(name);
	}

	/**
	 * 
	 */
	@Override
	public ArrayList<Integer> selectCards(int size) {
		int c1, c2;
		ArrayList<Integer> selection = new ArrayList<Integer>();
		System.out.printf("%sがカードを選んでいます．\n", name);
		
		c1 = r.nextInt(size) + 1;
		System.out.printf("1枚目:%d\n", c1);
		
		do {
			 c2 = r.nextInt(size) + 1;
		} while (c2 == c1);
		System.out.printf("2枚目:%d\n", c2);
		
		selection.add(c1);
		selection.add(c2);
		return selection;
	}
}
