
public class Main2 {

	public static void main(String[] args) {
		Line line = new Line(14);
		Box box = new Box(14, 6);
		Step step = new Step(14);
		
		box.setPen("＝");
		step.setPen("＊");
		
		line.display();
		box.display();
		step.display();
	}

}
