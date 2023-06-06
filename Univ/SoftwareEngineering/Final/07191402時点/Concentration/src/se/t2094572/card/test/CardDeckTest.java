package se.t2094572.card.test;

import junit.framework.TestCase;
import se.t2094572.card.entity.CardDeck;
import se.t2094572.card.entity.Card;
import java.util.ArrayList;
import java.util.List;

public class CardDeckTest extends TestCase {
	private CardDeck cd;
	private Card spadeA, diamond10, heartQ;
	private List<Card> list;
	
	protected void setUp() throws Exception {
		cd = new CardDeck();
		
		spadeA = new Card(0, 1); // スペードA
		diamond10 = new Card(1, 10); // ダイヤ10
		heartQ = new Card(2, 12); // ハートQ
		
		list = new ArrayList<Card>();
		
		cd.addCard(diamond10);
		cd.addCard(heartQ);
		cd.addCard(spadeA);
		
		list.add(diamond10);
		list.add(heartQ);
		list.add(spadeA);
		
	}

	protected void tearDown() throws Exception {
		
	}

	public void testTakeCard() {
		assertEquals(diamond10, cd.takeCard());
		assertEquals(heartQ, cd.takeCard());
		assertEquals(spadeA, cd.takeCard());
	}

	public void testTakeCardInt() {
		assertEquals(spadeA, cd.takeCard(3));
		assertEquals(heartQ, cd.takeCard(2));
		assertEquals(diamond10, cd.takeCard(1));
	}

	public void testSeeCard() {
		// 抜き取られていないことを確認するため、繰り返す。
		assertEquals(diamond10, cd.seeCard(1));
		assertEquals(heartQ, cd.seeCard(2));
		assertEquals(spadeA, cd.seeCard(3));
		assertEquals(diamond10, cd.seeCard(1));
		assertEquals(heartQ, cd.seeCard(2));
		assertEquals(spadeA, cd.seeCard(3));
	}

	public void testSearchCard() {
		assertEquals(3, cd.searchCard(0, 1));
		assertEquals(1, cd.searchCard(1, 10));
		assertEquals(2, cd.searchCard(2, 12));
		assertEquals(0, cd.searchCard(3, 5));
	}

	public void testIsEmpty() {
		assertFalse(cd.isEmpty());
		cd.clear();
		assertTrue(cd.isEmpty());
	}

	public void testSize() {
		assertEquals(3, cd.size());
	}

	public void testGetAllCards() {
		assertEquals(list, cd.getAllCards());
	}

}
