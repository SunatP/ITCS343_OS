# Multithreaded Programming (Week 3)

## Outline
   - What is Thread?
   - Thread vs Process
   - Thread Usage
   - Thread Implementation
   - Example: PThreads
   - Multithreading Models
   - Examples of Various Threads
   - Thread issues

## Thread
![Thread](https://www.javamex.com/tutorials/threads/ThreadDiagram.png)
   - A traditional or ***heavy-weight*** **process** is equal to one process with one thread
   - **Thread** คือหน่วยประมวลผลที่เล็กที่สุดของ และมันจะโดนจัดระเบียบสังคมโดย **Schecduler** ของ OS และภายใน thread จะประกอบไปด้วย **register** และ **stack**
   
   - Click on the YouTube content to learn about Process and Thread

  <a href="https://www.youtube.com/embed/O3EyzlZxx3g
" target="_blank"><img src="data:image/png;base64,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" 
alt="IMAGE ALT TEXT HERE" width="560" height="315" border="5" /></a>

## Process
![Process](https://i.ytimg.com/vi/bYF1XSJS8Qs/maxresdefault.jpg)   
   - คือส่วนหนึ่งๆ ของโปรแกรมที่กำลังทำงานอยู่ ใน Process จะประกอบด้วยโค้ด และ resources ต่างๆ การสร้าง Process จะขึ้นอยู่กับ OS และใน Process หนึ่งๆ อาจจะมีหลาย Threads ได้

## Process & Diagram
![PvsT](https://cdn-images-1.medium.com/max/1600/1*SQMYCFGyHdUxRBsHXvvteA.png)

## Thread vs. Process
   - One process may contain **more than one thread** (multithreaded process)
   - Some resources shared among threads belonging 
to the same process.
   - Resources are **not shared** among processes.
   - **Lightweight thread** vs. **Heavyweight process**

![tvsp](https://images.slideplayer.com/14/4402210/slides/slide_13.jpg)
   - **The different between Threads vs Process** 

![tvsp2](https://slideplayer.com/slide/6392122/22/images/14/Process+vs.+Thread+Process+Thread+Process+is+considered+heavy+weight.jpg)


## ข้อแตกต่างระหว่าง Process และ Thread
   1. **Address Space**
   จาก Diagram — Thread สามารถแชร์ Address Space ได้ โดยที่แต่ละ Threads ต้องอยู่ใน Process เดียวกัน, แต่ Thread จะมี Registers และ Stack เป็นของตัวเอง เพื่อใช้ทำ Context switching (Context switching ตัว OS จะเป็นคนจัดการ เมื่อต้องการเปลี่ยนการทำงานของ Thread)— แต่ Process มี Address Space เป็นของตัวเอง ไม่ได้แชร์กับใคร
   2. **Data and Segment**
   Thread ไม่มี Data และ Segment ของตัวเอง แต่ได้มาจากตัว Process ที่ครอบ Thread อยู่ จึงทำให้ Thread สามารถ แชร์ data และ segment ระหว่างกันได้ — ส่วน Process จะมี “สำเนา” ขอ Data และ Segment จาก Process ตัวแม่ การทำสำเนา คือการสร้างใหม่โดยใช้ค่าเดิม ของใหม่ที่สร้างมาถือว่าเป็นคนละตัวกัน ดังนั้นแม้สำเนาถูกแก้ Data และ Segment; Process ตัวแม่ จะไม่ถูกแก้ตามไปด้วย จึงทำให้ Process ไม่สามารถแชร์ข้อมูลกันได้โดยตรง
   3. **Communication**
   Thread ถูกออกแบบให้ สามารถติดต่อสื่อสารกันได้โดยตรงเมื่ออยู่ใน Process เดียวกัน — แต่ Process ไม่ได้ออกแบบมาให้ติดต่อสื่อสารได้โดยตรง ต้องทำผ่าน IPC (Inter-process communication) เพื่อให้ Process ลูกติดต่อกันเอง [Read more about IPC](https://en.wikipedia.org/wiki/Inter-process_communication)
   4. **Context Switching Overhead**
   Threads จะแชร์ Virtual Memory กันเองใน Process เดียวกัน เมื่อมีการเปลี่ยน Thread ในการทำงาน — Thread ใหม่ก็ใช้ Virtual Memory เดียวกันกับ Thread เก่า; Overhead ของการเปลี่ยน Thread ถือว่ามีน้อยมาก เป็นเลขที่ไม่มีนัยยะสักเท่าไหร่ — แต่ Process จะไม่ แชร์ Virtual Memory กัน การ Switch Process จะต้องทำการ Save State เก่าก่อน แล้ว Load State ของอีก Process ขึ้นมา ซึ่งส่วนนี้จะเป็น Overhead ที่เสียไป ดูลำดับการทำงานได้จาก Diagram ข้างล่าง
![CSO](https://cdn-images-1.medium.com/max/1600/1*y67yn_zqwILmW5nJNib3PQ.png)
จาก Process Switching diagram จะเห็นได้ว่า ตอนที่มีการ Switch Process จะมีการ Save/Load State ทุกครั้ง ซึ่ง State ในที่นี้รวมถึง Virtual Memory ด้วย จึงทำให้ CPU ต้องประมวลผลเยอะ — ในขณะที่ Thread Switching ก็มีการ ทำ Save/Load State เช่นกัน แต่มันใช้ Virtual Memory ร่วมกัน ดังนั้นค่าใช้จ่ายในการ Switch จะต่ำกว่ามาก เมื่อเทียบกับ Process
   5. **Creating**
   จากข้อ 4 และ Process and Thread diagram เราจะเห็นว่าองค์ประกอบของ Process จะมีเยอะกว่า Thead มาก ดังนั้นการสร้าง Thread ใหม่จึงทำได้ง่ายกว่า การสร้าง Process ใหม่ — การสร้าง Thread ใหม่สามารถทำได้ทันที เมื่ออยู่ใน Process ใดๆ แต่การสร้าง Process ใหม่จะต้องทำการ folk มาจาก Process เดิมก่อน (ทำสำเนา) จากนั้นถึงจะเริ่ม execute จึงจะได้ Process ใหม่
   6. **Controlling**
   Thread ใดๆ สามารถควบคุม Thread อื่นๆ ที่อยู่ใน Process เดียวกันได้ — แต่ Process ไม่สามารถควบคุม Process ใน Level เดียวกันได้ แต่ควบคุม Child Process ได้
   7.  **Change and Effect**
   การเปลี่ยนแปลงใน Main thread อาจส่งผลต่อ Thread อื่นๆ ใน Process เดียวกันได้; ในเมื่อ Thread ใช้ Virtual Memory ร่วมกัน ถ้า Thread ใดๆ สร้าง Crash ส่งผลให้ Process Crash และ Thread อื่นๆ Crash ไปด้วย — ในขณะที่ การเปลี่ยนแปลงของ Process จะไม่ส่งผลกับ Process อื่นๆ เนื่องจากมี Virtual Memory คนละตัวกัน นั้นคือ ถ้า Process A crash จะไม่ทำให้ Process B crash ไปด้วย ไม่ว่าจะเป็น Child/Parent กันหรือไม่ก็ตาม
## Pros and Cons.
   - **Good of Process**
      1. มี Virtual Memory เป็นของตัวเอง data ในแต่ละ Process จึงไม่กระทบกัน ความเสียหายจาก Process ใดๆ จะไม่ส่งผลต่อ Process อื่น
      2. มีความเป็นเจ้าของใน Resource ของตัวเอง
      3. ทุก process จะมี 1 light weight process (thread) เสมอ และสามารถมีหลาย Thread เพื่อช่วยในการทำงานได้
   - **Bad of Process** 
      1. Context switching สูง
      2. ต้องใช้ Inter Processing Communication(IPC) สูง
      3. สร้างได้ยากกว่า Thread และกิน Memory มากกว่า Thread
   - **Good of Thread**
      1. ใช้ Virtual Memory และ Share resource กัน
      2. Context Switching ต่ำ
      3. ติดต่อกันเองได้ระหว่าง Thread
      4. สร้างใหม่ง่ายกว่า กิน Memory น้อยกว่า
   -  **Bad of Thread**
      1. Resource ที่แชร์กัน ทำให้อาจจะมีการ read/write data พร้อมๆ กัน ข้อมูลในและ thread อาจจะไม่ Sync
      2. ถ้า thread ใดๆ เสียหาย จะส่งผลต่อ thread อื่นๆ ใน process เดียวกันด้วย และอาจทำให้ process เสียหายไปด้วย
      3. Thread คือหน่วยการทำงานที่เล็กที่สุดแล้ว ดังนั้นตัวมันเองจึงไม่สามารถมี Process ย่อยอื่นๆ มาช่วยทำงานได้อีก — แต่ยืมมือ Thread อื่น ได้นะ
## Thread Example
   - A word processor has three threads
   - First thread interacts with the **user**
   - Second thread handles **reformatting** in the background.
      - As soon as the sentence is deleted from page 1, the 
interactive thread tells the reformatting thread to 
reformat the whole book.
   - Third thread handles the **disk backups**

![ex](https://image.slidesharecdn.com/os-part-02-150330212852-conversion-gate01/95/processes-and-thread-ostanenbaum3e-24-638.jpg?cb=1427769057)

## Thread Usage
   - **Each thread has its own stack**
      - Stack contains one frame for each procedure called 
not yet returned from
      - Each thread will call different procedures and have 
different execution history
