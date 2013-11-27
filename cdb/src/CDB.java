import java.io.*;

/**
 * Generates example CDB statements.
 * @author Princeton Ferro
 *
 */
public class CDB {
	
	public static String getFileContents(String loc) throws IOException {
		BufferedReader b = new BufferedReader(new FileReader(loc));
		String text = "", nextLine = "";
		
		while ((nextLine = b.readLine()) != null)
			text += nextLine;
		
		b.close();
		return text;
	}
	
	public static void main(String[] args) {
		String exampleText = "A dog is a dog.";
	}

}
