






const char* GymStateFormat  = "{"
                                "\"key\": \"%s\" ,"
                                "\"ff\":{\"pos\":[%f,%f,%f],\"orn\":[%f,%f,%f]},"
                                "\"mf\":{\"pos\":[%f,%f,%f],\"orn\":[%f,%f,%f]},"
                                "\"rf\":{\"pos\":[%f,%f,%f],\"orn\":[%f,%f,%f]},"
                                "\"th\":{\"pos\":[%f,%f,%f],\"orn\":[%f,%f,%f]},"
                                "\"cube\":{\"pos\":[%f,%f,%f],\"orn\":[%f,%f,%f]}"
                                "}";

// const char* GymStateFormat  = "{%f,%f,%f},{%f,%f,%f}\n";
const char* GhostSateFormat = "{"
                                "\"key\": \"%s\" ,"
                                "\"ff\":%d,"
                                "\"mf\":%d,"
                                "\"rf\":%d,"
                                "\"th\":%d " 
                                "}";