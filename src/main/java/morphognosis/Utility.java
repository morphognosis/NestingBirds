// For conditions of distribution and use, see copyright notice in Morphognosis.java

// Utility class.

package morphognosis;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public abstract class Utility
{
   // Load integer.
   public static int loadInt(DataInputStream in) throws IOException
   {
      return(in.readInt());
   }


   // Load float.
   public static float loadFloat(DataInputStream in) throws IOException
   {
      return(in.readFloat());
   }


   // Load double.
   public static double loadDouble(DataInputStream in) throws IOException
   {
      return(in.readDouble());
   }


   // Save string.
   public static String loadString(DataInputStream in) throws IOException
   {
      return(in.readUTF());
   }


   // Save integer.
   public static void saveInt(DataOutputStream out, int value) throws IOException
   {
      out.writeInt(value);
   }


   // Save float.
   public static void saveFloat(DataOutputStream out, float value) throws IOException
   {
      out.writeFloat(value);
   }


   // Save double.
   public static void saveDouble(DataOutputStream out, double value) throws IOException
   {
      out.writeDouble(value);
   }


   // Save string.
   public static void saveString(DataOutputStream out, String value) throws IOException
   {
      out.writeUTF(value);
   }


   // Prevent instantiation.
   private Utility() {}
}
