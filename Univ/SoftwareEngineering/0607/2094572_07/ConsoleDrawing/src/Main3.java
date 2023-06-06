
public class Main3 {

	public static void main(String[] args) {

		Drawable pikachu = new Pikachu("Lv.100");
		Drawable cat = new Cat("888888888888");
		
		System.out.println(pikachu);
		pikachu.draw();
		
		System.out.println();
		
		System.out.println(cat);
		cat.draw();
	}

}
