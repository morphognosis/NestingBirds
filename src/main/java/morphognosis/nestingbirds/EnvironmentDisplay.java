// For conditions of distribution and use, see copyright notice in Morphognosis.java

// The nesting birds environment display.

package morphognosis.nestingbirds;

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Checkbox;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Label;
import java.awt.MediaTracker;
import java.awt.Point;
import java.awt.ScrollPane;
import java.awt.Scrollbar;
import java.awt.Toolkit;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class EnvironmentDisplay extends JFrame implements Runnable
{
   private static final long serialVersionUID = 0L;

   // Environment.
   public Environment environment;
	
   // Milliseconds between responses.
   static final int MIN_RESPONSE_DELAY = 100;
   static final int MAX_RESPONSE_DELAY = 1000;

   // Milliseconds between display updates.
   static final int DISPLAY_UPDATE_DELAY = 50;

   // World image dimensions.
   static final Dimension SCREEN_SIZE       = new Dimension(652, 600);
   static final Dimension CANVAS_SIZE       = new Dimension(850, 1100);
   static final Dimension STATUS_PANEL_SIZE = new Dimension(652, 100);
   static final Dimension CELL_SIZE         = new Dimension(30, 30);
   static final Dimension LOCALE_SIZE       = new Dimension(15, 15);
   static final Dimension OBJECT_SIZE       = new Dimension(20, 20);
   static final Dimension SMALL_BIRD_SIZE   = new Dimension(15, 15);
   static final Dimension BIRD_SIZE         = new Dimension(80, 80);

   // Screen size.
   Dimension screenSize;

   // Display.
   Graphics graphics;   
   ScrollPane canvasScroll;
   Dimension  canvasScrollSize;
   Canvas     canvas;
   Graphics   canvasGraphics;
   Image      worldImage;
   Graphics   worldImageGraphics;
   Dimension  worldImageSize;
   Point      femaleStatusLocation;
   Point      femaleImageLocation;
   Point      femaleObjectLocation;
   Point      maleStatusLocation;
   Point      maleImageLocation;
   Point      maleObjectLocation;
   Point      statusInfoOffset;
   Point      localeImageOffset;
   Point      objectImageOffset;
   Point      femaleImageOffset;
   Point      maleImageOffset;
   Thread displayThread;

   // Control panel.
   JPanel     controlPanel;
   Dimension controlPanelSize;
   Checkbox  stepButton;
   JSlider   responseSlider;
   int       response = MAX_RESPONSE_DELAY;

   // Images
   Image maleImage;
   Image maleSmallImage;
   Image femaleImage;
   Image femaleSmallImage;
   Image treeImage;
   Image cactusImage;
   Image mouseImage;
   Image stoneImage;
   Image eggImage;
   Image maleNetImage;
   Image femaleNetImage;

   // Font.
   Font        font = new Font("Helvetica", Font.BOLD, 12);
   FontMetrics fontMetrics;
   int         fontAscent;
   int         fontWidth;
   int         fontHeight;

   // Constructor.
   public EnvironmentDisplay(Environment environment)
   {
      this.environment   = environment;

      // Set up display.
      setTitle("Nesting birds");
      setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      setBounds(0, 0, 667, 655);
      setLayout(new GridLayout(1, 1));
      
      // Create display.
      screenSize = SCREEN_SIZE;
      setSize(screenSize);
      setLayout(new BorderLayout());
      canvasScroll     = new ScrollPane();
      canvasScrollSize = new Dimension(screenSize.width, (int)((double)screenSize.height * .95));
      canvasScroll.setBounds(0, 0, canvasScrollSize.width, canvasScrollSize.height);
      canvas = new Canvas();
      canvas.setBounds(0, 0, CANVAS_SIZE.width, CANVAS_SIZE.height);
      add(canvasScroll, BorderLayout.NORTH);
      canvasScroll.add(canvas, null);

      // Create control panel.
      controlPanel     = new JPanel();
      controlPanelSize = new Dimension(screenSize.width,
                                       (int)((double)screenSize.height * .05));
      controlPanel.setBounds(0, canvasScrollSize.height,
                             controlPanelSize.width, controlPanelSize.height);
      add(controlPanel);
      stepButton = new Checkbox("Step");
      controlPanel.add(stepButton);
      controlPanel.add(new Label("Fast", Label.RIGHT));
      responseSlider = new JSlider(Scrollbar.HORIZONTAL, MIN_RESPONSE_DELAY,
                                   MAX_RESPONSE_DELAY, MAX_RESPONSE_DELAY);
      responseSlider.addChangeListener(new responseSliderListener());
      controlPanel.add(responseSlider);
      controlPanel.add(new Label("Stop", Label.LEFT));

      // Get images.
      MediaTracker tracker = new MediaTracker(this);
      Image image   = loadImage("male.gif");
      tracker.addImage(image, 0);
      maleImage = image.getScaledInstance(BIRD_SIZE.width,
                                          BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(maleImage, 0);
      maleSmallImage = image.getScaledInstance(SMALL_BIRD_SIZE.width,
                                               SMALL_BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(maleSmallImage, 0);
      image = loadImage("female.gif");
      tracker.addImage(image, 0);
      femaleImage = image.getScaledInstance(BIRD_SIZE.width,
                                            BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(femaleImage, 0);
      femaleSmallImage = image.getScaledInstance(SMALL_BIRD_SIZE.width,
                                                 SMALL_BIRD_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(femaleSmallImage, 0);
      image = loadImage("tree.gif");
      tracker.addImage(image, 0);
      treeImage = image.getScaledInstance(LOCALE_SIZE.width,
                                          LOCALE_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(treeImage, 0);
      image = loadImage("cactus.gif");
      tracker.addImage(image, 0);
      cactusImage = image.getScaledInstance(LOCALE_SIZE.width,
                                            LOCALE_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(cactusImage, 0);
      image = loadImage("mouse.gif");
      tracker.addImage(image, 0);
      mouseImage = image.getScaledInstance(OBJECT_SIZE.width,
                                           OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(mouseImage, 0);
      image = loadImage("stone.gif");
      tracker.addImage(image, 0);
      stoneImage = image.getScaledInstance(OBJECT_SIZE.width,
                                           OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(stoneImage, 0);
      image = loadImage("egg.gif");
      tracker.addImage(image, 0);
      eggImage = image.getScaledInstance(OBJECT_SIZE.width,
                                         OBJECT_SIZE.height, Image.SCALE_DEFAULT);
      tracker.addImage(eggImage, 0);
      try
      {
         tracker.waitForAll();
      }
      catch (InterruptedException e) {
         status("Image loading interrupted");
         System.exit(1);
      }

      // Make display visible.
      pack();
      setLocation();
      setVisible(true);      
   }

   public void setLocation()
   {
      Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
      int       w   = getSize().width;
      int       h   = getSize().height;
      int       x   = (dim.width - w) / 4;
      int       y   = (dim.height - h) / 2;

      setLocation(x, y);
   }

   // Start.
   public void start()
   {
      // Create display update thread.
      if (displayThread == null)
      {
         displayThread = new Thread(this);
         displayThread.start();
      }
   }


   // Stop.
   public void stop()
   {
      if (displayThread != null)
      {
         displayThread = null;
      }
   }


   // Run.
   public void run()
   {
      // Lower this thread's priority and get the current time.
      Thread.currentThread().setPriority(Thread.MIN_PRIORITY);

      // Display update loop.
      while (Thread.currentThread() == displayThread)
      {
         updateDisplay();

         try
         {
            Thread.sleep(DISPLAY_UPDATE_DELAY);
         }
         catch (InterruptedException e) { break; }
      }
   }

   // Update display.
   public void updateDisplay()
   {
      int    x, y, x2, y2, i;
      String s;
      
      // Initialize graphics.
      if (graphics == null)
      {
    	  if (!initGraphics()) return;
      }
      
      // Clear.
      worldImageGraphics.setColor(Color.white);
      worldImageGraphics.fillRect(0, 0, worldImageSize.width, worldImageSize.height);

      // Draw world.
      worldImageGraphics.setColor(Color.black);
      y2 = worldImageSize.height;
      int h = STATUS_PANEL_SIZE.height;
      for (x = x2 = 0; x < Environment.width; x++, x2 += CELL_SIZE.width)
      {
         worldImageGraphics.drawLine(x2, h, x2, y2);
      }
      worldImageGraphics.drawLine(x2, h, x2, y2);
      x2 = Environment.width * CELL_SIZE.width;
      for (y = 0, y2 = h; y < Environment.width; y++, y2 += CELL_SIZE.height)
      {
         worldImageGraphics.drawLine(0, y2, x2, y2);
      }
      worldImageGraphics.drawLine(0, y2, x2, y2);
      for (x = x2 = 0; x < Environment.width; x++, x2 += CELL_SIZE.width)
      {
         for (y = 0, y2 = h; y < Environment.height; y++, y2 += CELL_SIZE.height)
         {
            switch (environment.world[x][y].locale)
            {
            case Environment.LOCALE.DESERT:
               worldImageGraphics.drawImage(cactusImage, x2 + localeImageOffset.x,
                                            y2 + localeImageOffset.y, this);
               break;

            case Environment.LOCALE.FOREST:
               worldImageGraphics.drawImage(treeImage, x2 + localeImageOffset.x,
                                            y2 + localeImageOffset.y, this);
               break;

            case Environment.LOCALE.PLAIN:
               break;
            }

            switch (environment.world[x][y].object)
            {
            case Environment.OBJECT.NO_OBJECT:
               break;

            case Environment.OBJECT.EGG:
               worldImageGraphics.drawImage(eggImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;

            case Environment.OBJECT.MOUSE:
               worldImageGraphics.drawImage(mouseImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;

            case Environment.OBJECT.STONE:
               worldImageGraphics.drawImage(stoneImage, x2 + objectImageOffset.x,
                                            y2 + objectImageOffset.y, this);
               break;
            }
         }
      }

      // Female status.
      worldImageGraphics.drawImage(femaleImage, femaleImageLocation.x,
                                   femaleImageLocation.y, this);
      s = "Female";
      x = femaleImageLocation.x + ((BIRD_SIZE.width - fontMetrics.stringWidth(s)) / 2);
      y = femaleImageLocation.y + BIRD_SIZE.height + fontAscent;
      worldImageGraphics.drawString(s, x, y);
      if (environment.female.hasObject == Environment.OBJECT.MOUSE)
      {
         worldImageGraphics.drawImage(mouseImage, femaleObjectLocation.x,
                                      femaleObjectLocation.y, this);
      }
      else if (environment.female.hasObject == Environment.OBJECT.STONE)
      {
         worldImageGraphics.drawImage(stoneImage, femaleObjectLocation.x,
                                      femaleObjectLocation.y, this);
      }      
      s = "Sensors: [" + environment.female.sensorsToString() + "]";
      x = femaleStatusLocation.x + statusInfoOffset.x;
      y = femaleStatusLocation.y + statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      s = "Orientation: " + Bird.orientationToString(environment.female.orientation);
      y += fontAscent + 2;
      worldImageGraphics.drawString(s, x, y);      
      s = "Response (#" + environment.female.response + "): " +
          responseToString(environment.female.response);
      y += fontAscent + 2;
      worldImageGraphics.drawString(s, x, y);
      s = "Food: " + environment.female.food;
      y += fontAscent + 2;
      worldImageGraphics.drawString(s, x, y);
      x2 = environment.female.x * CELL_SIZE.width;
      y2 = (environment.female.y * CELL_SIZE.height) + h;
      worldImageGraphics.drawImage(femaleSmallImage, x2 + femaleImageOffset.x,
                                   y2 + femaleImageOffset.y, this);

      // Male status.
      worldImageGraphics.drawImage(maleImage, maleImageLocation.x,
                                   maleImageLocation.y, this);
      s = "Male";
      x = maleImageLocation.x + ((BIRD_SIZE.width - fontMetrics.stringWidth(s)) / 2);
      y = maleImageLocation.y + BIRD_SIZE.height + fontAscent;
      worldImageGraphics.drawString(s, x, y);
      if (environment.male.hasObject == Environment.OBJECT.MOUSE)
      {
         worldImageGraphics.drawImage(mouseImage, maleObjectLocation.x,
                                      maleObjectLocation.y, this);
      }
      else if (environment.male.hasObject == Environment.OBJECT.STONE)
      {
         worldImageGraphics.drawImage(stoneImage, maleObjectLocation.x,
                                      maleObjectLocation.y, this);
      }
      s = "Sensors: [" + environment.male.sensorsToString() + "]";
      x = maleStatusLocation.x + statusInfoOffset.x;
      y = maleStatusLocation.y + statusInfoOffset.y;
      worldImageGraphics.drawString(s, x, y);
      s = "Orientation: " + Bird.orientationToString(environment.male.orientation);
      y += fontAscent + 2;
      worldImageGraphics.drawString(s, x, y);       
      s = "Response (#" + environment.male.response + "): " +
          responseToString(environment.male.response);
      y += fontAscent + 2;
      worldImageGraphics.drawString(s, x, y);
      s = "Food: " + environment.male.food;
      y += fontAscent + 2;
      worldImageGraphics.drawString(s, x, y);
      x2 = environment.male.x * CELL_SIZE.width;
      y2 = (environment.male.y * CELL_SIZE.height) + h;
      worldImageGraphics.drawImage(maleSmallImage, x2 + maleImageOffset.x,
                                   y2 + maleImageOffset.y, this);

      // Copy to display.
      canvasGraphics.drawImage(worldImage, 0, 0, this);
   }
   
   // Initialize graphics.
   boolean initGraphics()
   {
	  int x,y;
	  
	  if (graphics != null) return true;
      graphics = getGraphics();
      if (graphics == null) return false;
      
      // Initialize font.
      graphics.setFont(font);
      fontMetrics = graphics.getFontMetrics();
      fontAscent  = fontMetrics.getMaxAscent();
      fontWidth   = fontMetrics.getMaxAdvance();
      fontHeight  = fontMetrics.getHeight();	   
      
       // Initialize graphics.
       if ((x = Environment.width * CELL_SIZE.width) < STATUS_PANEL_SIZE.width)
       {
          x = STATUS_PANEL_SIZE.width;
       }
       x++;
       y = (Environment.height * CELL_SIZE.height) + STATUS_PANEL_SIZE.height;
       worldImageSize     = new Dimension(x, y);
       worldImage         = createImage(x, y);
       worldImageGraphics = worldImage.getGraphics();
       y = 1;
       femaleStatusLocation = new Point(0, y);
       x = (int)((STATUS_PANEL_SIZE.width - (2.5 * BIRD_SIZE.width)) / 2);
       femaleImageLocation = new Point(x, y);
       x = femaleImageLocation.x + ((BIRD_SIZE.width * 7) / 8);
       y = femaleImageLocation.y + (BIRD_SIZE.height / 8);
       femaleObjectLocation = new Point(x, y);
       x = 1;
       y = CELL_SIZE.height - SMALL_BIRD_SIZE.height;
       femaleImageOffset = new Point(x, y);
       x = (int)(femaleImageLocation.x + (1.5 * BIRD_SIZE.width));
       y = 1;
       maleImageLocation = new Point(x, y);
       x = maleImageLocation.x + BIRD_SIZE.width;
       maleStatusLocation = new Point(x, y);
       x = maleImageLocation.x - (BIRD_SIZE.width / 8) - 1;
       y = maleImageLocation.y + (BIRD_SIZE.height / 8);
       maleObjectLocation = new Point(x, y);
       x = CELL_SIZE.width - SMALL_BIRD_SIZE.width;
       y = CELL_SIZE.height - SMALL_BIRD_SIZE.height;
       maleImageOffset = new Point(x, y);
       x = 2;
       y = 2 + fontAscent;
       statusInfoOffset  = new Point(x, y);
       localeImageOffset = new Point(1, 1);
       x = (CELL_SIZE.width - OBJECT_SIZE.width) / 2;
       y = (CELL_SIZE.height - OBJECT_SIZE.height) / 2;
       objectImageOffset = new Point(x, y);	
       canvasGraphics = canvas.getGraphics();       
	   return true;
   }


   // Response to string.
   public String responseToString(int response)
   {
	  String s = MaleBird.responseToString(response);
	  if (s == null)
	  {
		  s = FemaleBird.responseToString(response);
	  }
	  return s;
   }


   // View choice listener.
   class viewChoiceItemListener implements ItemListener
   {
      public void itemStateChanged(ItemEvent evt)
      {
         // Clear canvas for new image.
         canvasGraphics.setColor(Color.white);
         canvasGraphics.fillRect(0, 0, CANVAS_SIZE.width, CANVAS_SIZE.height);
      }
   }

   // Response slider listener.
   class responseSliderListener implements ChangeListener
   {
      public void stateChanged(ChangeEvent evt)
      {
         response = responseSlider.getValue();
      }
   }

   // Load image from file.
   Image loadImage(String name)
   {
      String filename = "res/images/" + name;
      Image image = null;
      try 
      {
         image = ImageIO.read(new File(filename));
      }
      catch (IOException e) 
      {
         status("Cannot load image " + name);    	  
      }
      return(image);
   }
   
   // Status message.
   void status(String message)
   {
	   System.out.println(message);
   }

   // Main.
   public static void main(String[] args)
   {	   
	  // Create environment.
	  Environment environment = new Environment();
	  
      // Create environment display.
      EnvironmentDisplay environmentDisplay = new EnvironmentDisplay(environment);

      // Start.
      environmentDisplay.start();
   }
}
