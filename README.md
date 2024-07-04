# Check Permission

This small utility program is designed to check directory permissions for a specified user on a Linux operating system.

## Usage

To run the program, use the following command:

```shell
mycheckperm args...
```

## Explanation

* `-p`: Specify the starting path for recursive access checking.
* `-g`: Specify group access permissions.
* `-u`: Specify user access permissions. This option automatically considers the group associated with the selected user.

## Example Usage

To check permissions for a specific user starting from a particular directory recursively:

```shell
mycheckperm -p /path/to/start/dir -u username
```

To check permissions for a specific group:

```shell
mycheckperm -p /path/to/start/dir -g groupname
```

## Notes

* Ensure the program is run with appropriate privileges to access the specified directories.
* The program assumes a Linux environment and may not behave as expected on other operating systems.

## License

This program is licensed under the MIT License. See LICENSE.txt for details.