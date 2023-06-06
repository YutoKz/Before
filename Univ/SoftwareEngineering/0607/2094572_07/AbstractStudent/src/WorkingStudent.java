
public class WorkingStudent extends Student {
	
	private String company;
	
	public WorkingStudent()
	{
		super();
	}
	
	public WorkingStudent(String id, String name, int credit, String company)
	{
		super(id, name, credit);
		this.setCompany(company);
	}
	
	
	
	@Override
	public void explain()
	{
		System.out.println("私" + getName() + "は、" + company + "に勤務する社会人学生です。");		
	}
	
	public void setCompany(String company) {
		this.company = company;
	}

}
