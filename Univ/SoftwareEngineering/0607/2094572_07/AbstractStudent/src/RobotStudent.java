
public class RobotStudent extends Student{

	private String vendor;
	private String algorithm;
	
	public RobotStudent()
	{
		super();
	}
	
	public RobotStudent(String id, String name, int credit, String vendor, String algorithm)
	{
		super(id, name, credit);
		this.setVendor(vendor);
		this.setAlgorithm(algorithm);
	}
	
	
	
	@Override
	public void explain()
	{
		System.out.println("私" + getName() + "は、" + vendor + "製のロボット学生です。" + algorithm + "で動いています。");
	}
	
	
	
	public void setVendor(String vendor) {
		this.vendor = vendor ;
	}

	public void setAlgorithm(String algorithm) {
		this.algorithm = algorithm;
	}
}
