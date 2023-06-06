
public class InternationalStudent extends Student{

	private String country;
	private boolean kokuhi;
	
	public InternationalStudent()
	{
		super();
	}
	
	public InternationalStudent(String id, String name, int credit, String country, boolean kokuhi)
	{
		super(id, name, credit);
		this.setCountry(country);
		this.setKokuhi(kokuhi);
	}
	
	
	
	@Override
	public void explain()
	{
		System.out.print("私" + getName() + "は、" + country + "から来た");
		if (kokuhi == true) {
			System.out.println("国費留学生です．");
		} else {
			System.out.println("私費留学生です．");
		}
	}
	
	
	
	public void setCountry(String country) {
		this.country = country;
	}

	public void setKokuhi(boolean kokuhi) {
		this.kokuhi = kokuhi;
	}
}
