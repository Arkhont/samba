/*
   Unix SMB/CIFS implementation.
   RPC pipe client

   Copyright (C) Gerald (Jerry) Carter          2004.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Stupid dummy functions required due to the horrible dependency mess
   in Samba. */

#include "includes.h"

int find_service(TALLOC_CTX *ctx, const char *service_in, char **p_service_out)
{
	return -1;
}

bool conn_snum_used(struct smbd_server_connection *sconn,
		    int snum)
{
	return False;
}
