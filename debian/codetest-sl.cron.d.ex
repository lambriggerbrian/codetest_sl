#
# Regular cron jobs for the codetest-sl package
#
0 4	* * *	root	[ -x /usr/bin/codetest-sl_maintenance ] && /usr/bin/codetest-sl_maintenance
