#include <vector>
#include <iostream>
#include <cstdlib>
#include <boost/process.hpp>
#include <libbutl/process.hxx>

int main (int argc, char* argv[])
{
    if(argc != 2)
    {
        return EXIT_FAILURE;
    }

    static const char* command_to_run = "cl fioniofiosdnf.cpp /diagnostics:color";

    std::string command = argv[1];

    if(command == "cmd")
        return std::system(command_to_run);

    if(command == "popen")
    {
        // Source: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/popen-wpopen?view=msvc-170

        char psBuffer[128];
        FILE* pPipe;

        /* Run DIR so that it writes its output to a pipe. Open this
        * pipe with read text attribute so that we can read it
        * like a text file.
        */

        if ((pPipe = _popen(command_to_run, "rt")) == NULL)
        {
            exit(1);
        }

        /* Read pipe until end of file, or an error occurs. */

        while (fgets(psBuffer, 128, pPipe))
        {
            puts(psBuffer);
        }

        int endOfFileVal = feof(pPipe);
        int closeReturnVal = _pclose(pPipe);

        if (endOfFileVal)
        {
            printf("\nProcess returned %d\n", closeReturnVal);
        }
        else
        {
            printf("Error: Failed to read the pipe to the end.\n");
        }
    }

    if(command == "boost.process-cmd")
    {
        namespace bp = boost::process;
        bp::system(command_to_run, bp::std_out > stdout, bp::std_err > stderr, bp::std_in < stdin);
    }

    if(command == "boost.process-forward")
    {
        namespace bp = boost::process;
        bp::child child_process{command_to_run, bp::std_out > stdout, bp::std_err > stderr, bp::std_in < stdin};
        child_process.wait();
    }

    if(command == "boost.process-spawn")
    {
        namespace bp = boost::process;
        std::vector<std::string_view> command_args;
        boost::algorithm::split(command_args, command_to_run, boost::algorithm::is_any_of(" "));
        const auto program_to_run = bp::search_path(command_args[0]);
        const std::vector<std::string_view> args(std::next(command_args.begin()), command_args.end());
        bp::spawn(program_to_run, bp::args(args) , bp::std_out > stdout, bp::std_err > stderr, bp::std_in < stdin);
    }

    if(command == "libbutl.process")
    {
        std::vector<std::string> command_args;
        boost::algorithm::split(command_args, command_to_run, boost::algorithm::is_any_of(" "));
        std::vector<const char *> command_args_c;
        for(const auto& str : command_args)
            command_args_c.push_back(str.c_str());
        butl::process child_process{command_args_c}; //, 0, -1, 2};
        child_process.wait();
    }


    return EXIT_FAILURE;
}
