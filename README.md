# vlang, A Distributed Programming Language

1. User describes the infrastructure by graph, how many pods we want, how to access the pods
2. Programming language provides communication support on pods level, for fast-small, slow-large, shared solutions
3. The communication supports are provided by shared memory cache, database, cloud storage, etc.
4. The programming language will create the infrastructure for those non-user facing items like database, cache, etc., doesn't need the programmer to define manually
5. Frontend is also considered as part of the system, so the communication between frontend and backend will be involved as well
6. User can describe what communication is required among the components and how fatal each module is, the programming language will create a infrastructure for the user automatically
7. Security, keys, tokens, certificates will automatically be generated and used for communication, no manual definition is required

## Example of System Description

1. user_login_page: runs on any browser, can talk to: [login_api:fast, help_api]
2. user_browse_edit_page: runs on any browser, can talk to: [get_details_api:fast, list_api:fast, update_api:fast, status_api:realtime]
3. login_api: runs on login_server, can talk to: [oauth_server:realtime, user_profile_server:fast]
4. help_api: runs on login_server, can talk to: []
5. get_details_api: runs on business_server, can talk to: [oauth_server:realtime, database_server:realtime, hub_server:realtime]
6. list_api: runs on business_server, can talk to: [oauth_server:realtime, database_server:realtime, hub_server:realtime]
7. update_api: runs on business_server, can talk to:  [oauth_server:realtime, database_server:realtime, hub_server:realtime]
8. status_api: runs on business_server, can talk to:  [oauth_server:realtime, hub_server:realtime]
9. oauth_server: jwt_token_verification server
10. hub_server: redis cache key-value pair service
11. database_server: postgresql database service

