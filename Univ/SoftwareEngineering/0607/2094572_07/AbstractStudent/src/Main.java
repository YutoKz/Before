
public class Main {

	public static void main(String[] args) {
		
		Student s = new RegularStudent("1111111A", "佐藤", 136);
		Student is = new InternationalStudent("2222222B", "George", 80, "アメリカ", true);
		Student ws = new WorkingStudent("3333333C", "伊藤", 100, "神戸コーポレーション");
		Student rs = new RobotStudent("4444444D", "ジョーチ", 36, "神大工業", "ニューラルネット");
		
		s.hello();
		is.hello();
		ws.hello();
		rs.hello();
		
		System.out.println();

		s.graduate();
		is.graduate();
		ws.graduate();
		rs.graduate();
		
		System.out.println();
		
		s.addCredit(50);
		is.addCredit(50);
		ws.addCredit(50);
		rs.addCredit(50);
		
		System.out.println();
		
		s.graduate();
		is.graduate();
		ws.graduate();
		rs.graduate();
	}

}
