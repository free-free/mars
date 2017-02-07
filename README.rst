mars
====================
A serial port debugging assistant written in Qt


ui view
---------

comannd line window
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: http://okzdacydq.bkt.clouddn.com/cmd.png
  :align: center

graph window
^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: http://okzdacydq.bkt.clouddn.com/figure.png
  :align: center

serial port config window
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: http://okzdacydq.bkt.clouddn.com/serialportconfig.png
  :align: center

requirements
----------------

**Qt**: > 4


build
----------

building a windows executable using mxe on a linux system
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. get mxe 
++++++++++++++++++

.. code-block:: bash

    $ git clone https://github.com/mxe/mxe.git


2. install build dependencies
+++++++++++++++++++++++++++++++++

`dependencies <http://mxe.cc/#requirements>`_


3. Build Qt 5 for windows
+++++++++++++++++++++++++++++++

.. code-block:: bash

    $ cd mxe && make qtbase qtserialport    


4. Get to the directory of your app, and run the following command
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. code-block:: bash

    $ <your mxe root directory>/usr/bin/i686-w64-mingw32.static-qmake-qt5


5. Build your project
+++++++++++++++++++++++++++++

.. code-block:: bash

    $ make


6. get your windows executable in the sub directory 'Release' of your project directory
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




LICENSE
----------------
       
`MIT LICENSE <LICENSE>`_
