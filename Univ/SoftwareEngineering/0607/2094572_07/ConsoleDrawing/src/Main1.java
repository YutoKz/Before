
public class Main1 {

	public static void main(String[] args) {
		
		Line line1 = new Line(20);
		
		line1.display();
		
		line1.setPen("＝");
		line1.draw();
		line1.setPen("＊");
		line1.draw();
		line1.setPen("＋");
		line1.draw();
		line1.setPen("◇");
		line1.draw();
		
		line1.setLength(10);
		line1.display();
		
		Line line2 = new Line(10);
		line2.display();
		
		for(int i = 0; i < 3; i++)
		{
			line1.draw();
			line2.draw();
		}
		
		
		
		Box box = new Box(10, 6);
		
		box.display();
		
		box.setPen("○");
		box.draw();
	}

}
