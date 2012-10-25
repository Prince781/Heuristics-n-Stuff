//A simple Tic-Tac-Toe game tree generator
import java.io.*;
import java.util.*;
import java.util.regex.*;
import javax.swing.*;

public class T3Tree {
	public static boolean verbose = false;
	public static String bd_to_string(char[][] bd) {
		String str = "";
		for (int y=0; y<3; y++)
			for (int x=0; x<3; x++)
				str = str+bd[y][x];
		return str;
	}
	public static char[][] build_board(String gamestate) {
		char[][] crds = new char[3][3]; //[y][x]
		int gpos = 0;
		if (verbose) {
			System.out.println("  Board:  ");
			System.out.println("----------");
		}
		for (int y=1; y<=3; y++) {
			if (verbose) System.out.println(" ");
			for (int x=1; x<=3; x++) {
				crds[(x-1)][(y-1)] = gamestate.charAt(gpos);
				if (verbose) System.out.print(gamestate.charAt(gpos)+"   ");
				gpos++;
			}
			if (verbose) System.out.println(" ");
		}
		if (verbose) System.out.println("----------");
		return crds;
	}
	public static int[] yxpos_from_int(int pos) {
		int y = (int)Math.ceil(pos/3f); //get height
		int x = pos-(3*(y-1)); //get pos x
		int[] rval = {y,x};
		return rval;
	}
	public static int getState(String gamestate) {
		//returns values: -1=p win, 0=draw/nothing, 1=c win, -2=error
		//board values: x=player, o=computer, n=nothing
		char[][] crds = build_board(gamestate);
		int sVal = 0;
		for (int i=1; i<=gamestate.length(); i++) {
			char c = gamestate.charAt(i-1);
			if (c=='n') continue; //don't assess empty squares
			int y = yxpos_from_int(i)[0];
			int x = yxpos_from_int(i)[1];
			boolean reqx = true;
			boolean reqy = true;
			boolean reqd1 = true; //diagonal
			boolean reqd2 = true;
			for (int a=0; a<crds.length; a++) //loop through x, where y=y
				if (crds[a][(y-1)]!=c)
					reqy = false; //no match on line y
			for (int a=0; a<crds[(x-1)].length; a++) //loop through y, where x=x
				if (crds[(x-1)][a]!=c)
					reqx = false; //no match on line x
			for (int a=0; a<3; a++) //loop through x,y, where y=x
				if (crds[a][a]!=c)
					reqd1 = false;
			for (int a=2;a>=0;a--) //loop through x,y, where y=-x
				if (crds[a][(2-a)]!=c)
					reqd2 = false;
			if (reqx||reqy||reqd1||reqd2) {
				sVal = (c=='x'?-1:1);
				break;
			}
		}
		return sVal;
	}
	public static String[] array_no_duplicates(String[] arr) { //remove duplicates from an array
		/*
		System.out.println("Original array is: ");
		for (int n=0; n<arr.length; n++)
			System.out.print((n==0?"{":", ")+arr[n]+(n==arr.length-1?"}":""));
		System.out.println();
		*/
		for (int i=0; i<arr.length; i++) {
			for (int j=0; j<arr.length; j++) {
				if (i==j) continue;
				if (arr[i].equals(arr[j])) { //remove duplicate
					String[] fh = new String[j];
					String[] sh = new String[arr.length-j-1]; 
					System.arraycopy(arr,0,fh,0,j);
					System.arraycopy(arr,j+1,sh,0,arr.length-j-1);
					arr = new String[fh.length+sh.length];
					System.arraycopy(fh,0,arr,0,fh.length);
					System.arraycopy(sh,0,arr,fh.length,sh.length);
					j--;
				}
			}
		}
		/*
		System.out.println("Final array is: ");
		for (int n=0; n<arr.length; n++)
			System.out.print((n==0?"{":", ")+arr[n]+(n==arr.length-1?"}":""));
		System.out.println();
		*/
		return arr;
	}
	public static String build_tree(String gstate) { //second (failed) version of build_tree()
		if (getState(gstate)!=0) return gstate;
		boolean cmoving = true;
		Matcher m = Pattern.compile("n").matcher(gstate);
		int tl = 0;
		while (m.find()) tl++;
		String[] tree = new String[tl];
		String[] stem = new String[tl];
		m = Pattern.compile("n").matcher(gstate);
		int mn = 0;
		while (m.find()) {
			String bst = gstate.substring(0,m.start())+(cmoving?'o':'x')+gstate.substring(m.end(),gstate.length());
			tree[mn] = bst+"_"+mn;
			stem[mn] = bst;
			mn++;
		}
		String[][] leaf_moves = new String[stem.length][0];
		String[] next_tree = new String[0];
		while (tree.length > 0) {
			cmoving = !cmoving;
			for (int a=0; a<tree.length; a++) {
				Matcher mat = Pattern.compile("n").matcher(tree[a]);
				String[] matches = new String[0];
				while (mat.find()) {
					String ostr = tree[a].substring(0,mat.start())+(cmoving?'o':'x')+tree[a].substring(mat.end(),tree[a].length()); //string
					String cst = ostr.substring(0,ostr.length()-2); //current state
					int lpos = Integer.parseInt(tree[a].substring(tree[a].length()-1,tree[a].length()));
					if (getState(cst)!=0) { //add to leaf_moves[lpos]
						String[] lcopy = new String[leaf_moves[lpos].length+1];
						System.arraycopy(leaf_moves[lpos],0,lcopy,0,leaf_moves[lpos].length);
						lcopy[lcopy.length-1] = cst;
						leaf_moves[lpos] = lcopy;
					} else { //add to matches
						String[] mcopy = new String[matches.length+1];
						System.arraycopy(matches,0,mcopy,0,matches.length);
						mcopy[mcopy.length-1] = ostr;
						matches = mcopy;
					}
				}
				String[] next_treecopy = new String[next_tree.length+matches.length]; //add to next_tree
				System.arraycopy(next_tree, 0, next_treecopy, 0, next_tree.length);
				System.arraycopy(matches, 0, next_treecopy, next_tree.length, matches.length);
				next_tree = next_treecopy;
			}
			for (int b=0; b<leaf_moves.length; b++)
				leaf_moves[b] = array_no_duplicates(leaf_moves[b]); //clean duplicates
			next_tree = array_no_duplicates(next_tree); //clean duplicates
			tree = next_tree;
			next_tree = new String[0];
		}
		int[] mvvalues = new int[leaf_moves.length];
		Arrays.fill(mvvalues, 0);
		for (int c=0; c<leaf_moves.length; c++)
			for (int d=0; d<leaf_moves[c].length; d++)
				mvvalues[c] += getState(leaf_moves[c][d]);
		//for (int e=0; e<mvvalues.length; e++)
		//	System.out.println(stem[e]+" = "+mvvalues[e]);
		String[] next_moves = new String[0];
		int f=0;
		while(next_moves.length < mvvalues.length) {
			boolean lval = false;
			for (int g=0; g<mvvalues.length; g++) {
				if (g==f) continue;
				if (mvvalues[g]>mvvalues[f]) {
					lval = true;
					f = g;
					break;
				}
			}
			if (lval) continue; //if there is no larger value
			String[] next_movescopy = new String[next_moves.length+1]; //add new move
			System.arraycopy(next_moves,0,next_movescopy,0,next_moves.length);
			next_movescopy[next_movescopy.length-1] = stem[f];
			next_moves = next_movescopy; 
			f = (f+1 > mvvalues.length-1 ? 0 : f+1);
		}
		//for (int h=0; h<next_moves.length; h++)
		//	System.out.println(next_moves[h]);
		return (next_moves.length > 0 ? next_moves[0] : gstate); //most viable move
	}
	public static void main(String[] args) throws IOException {
		GUI gui = new GUI("Tic-Tac-Toe");
		SwingUtilities.invokeLater(gui); //set to run
		//BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		//System.out.println(build_tree(br.readLine()));
	}
}