/// delegator.hpp
/// Shaun Harker 
/// 2011

#include <set>
#include <deque>
#include <unistd.h>

/***************************************
 *         USER INTERFACE              *
 ***************************************/

namespace delegator {
  
  inline void Start ( void ) {
  	/* Initialize the MPI communications */
  	int argc = 0; char * * argv = NULL;
		MPI_Init(&argc, &argv); 
  }
  
  template < class Process >
  int Run ( void ) {
    int argc = 0; char * * argv = NULL;
    return Run < Process > ( argc, argv );
  } /* Run<> */

  inline void Stop ( void ) {
  	/* Finalize the MPI communications. */
		MPI_Finalize();
  }
  
  template < class Process >
  int Run ( int argc, char * argv [] ) {
    typedef Coordinator_Worker_Scheme Scheme;
    return Run < Process, Scheme, Communicator > ( argc, argv );
  } /* Start<> */

  // Run (more advanced interface)
  template < class Process, class Scheme, class Comm >
  int Run ( int argc, char * argv [] ) {
    /* Create Process, Scheme, and Communicator */
    Comm my_communicator;
    Scheme my_scheme ( argc, argv );
    Process my_process;
    
    // Run Scheme until it finishes
    my_communicator . initialize ();
    std::thread t ( &Scheme::run, &my_scheme, &my_process, &my_communicator ); //my_scheme . run ( & my_process, & my_communicator );
    my_communicator . daemon ();
    //std::cout << "daemon returned\n";
    t . join ();
    //std::cout << "thread joined\n";
    my_communicator . finalize ();
    //std::cout << "communicator finalized\n";
    return 0; 
  } /* Run<> */
}


