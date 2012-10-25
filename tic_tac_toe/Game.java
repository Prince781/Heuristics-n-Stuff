import java.util.regex.*;


public class Game {
	public String state = "nnnnnnnnn";
	boolean cturn = false; //false - player, true - computer
	boolean over = false;
	public void move(int tile) { //player movement
		if (over||cturn||state.charAt(tile) == 'o'||state.charAt(tile) == 'x') return;
		String nst = state.substring(0,tile)+"x"+state.substring(tile+1,state.length()); //generate new state
		state = nst; //update the game state
		cturn = !cturn;
		computer_move();
	}
	public void computer_move(){
		if (over || !cturn) return;
		System.out.println("State is "+state);
		System.out.println(" Next state is "+T3Tree.build_tree(state));
		state = T3Tree.build_tree(state);
		cturn = !cturn; //switch players
		//check to see if over
		Matcher m = Pattern.compile("n").matcher(state);
		int cn = 0;
		while (m.find()) cn++;
		int st = T3Tree.getState(state);
		if (cn==0||st!=0) { //game is over
			over = true;
			System.out.println("Game is over.");
			if (st != 0)
				System.out.println(st==1?"Computer won.":"You won!");
		}
	}
}
